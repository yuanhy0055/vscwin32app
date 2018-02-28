#include <iostream>

#include <windows.h>
#include <winsock.h>
#include <sys/stat.h>
#include <io.h>
#include <conio.h>

char *yy_rcs_revstr = "$Revision: 100 $";
char *yy_revstr = NULL;
int major_verson = 1;
int minor_verson = 0;

#ifdef _MSC_VER
#define snprintf c99_snprintf

inline int c99_vsnprintf(char* str, size_t size, const char* format, va_list ap)
{
    int count = -1;

    if (size != 0)
        count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
    if (count == -1)
        count = _vscprintf(format, ap);

    return count;
}
inline int c99_snprintf(char* str, size_t size, const char* format, ...)
{
    int count;
    va_list ap;

    va_start(ap, format);
    count = c99_vsnprintf(str, size, format, ap);
    va_end(ap);

    return count;
}
#endif // _MSC_VER

void
fatal( const char *fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    fprintf(stderr, "FATAL: ");
    vfprintf( stderr, fmt, args );
    va_end( args );
    exit (EXIT_FAILURE);
}

void *ymalloc (size_t size)
{
    void *ret = malloc(size);
    if (ret == NULL)
	fatal("Cannot allocate memory: %d bytes.\n", size);
    return ret;
}

void yy_make_revstr(void)
{
    char *ptr;
    size_t len;
    ptr = strstr(yy_rcs_revstr, ": ");
    if (!ptr) {
        yy_revstr = strdup("unknown");
        return;
    }
    ptr += 2;
    /* assume subversion's keyword expansion like "Revision: 96". */
    minor_verson = atoi(ptr);
    yy_revstr = (char *)ymalloc(20);
    snprintf(yy_revstr, 20, "%d.%d", major_verson, minor_verson);
}

void yy_debug( const char *fmt, ... )
{
    va_list args;
    if ( f_debug ) {
        va_start( args, fmt );
        fprintf(stderr, "DEBUG: ");
        vfprintf( stderr, fmt, args );
        va_end( args );
    }
}

int r0_main(void)
{
	std::cout << "Hello" << std::endl;
	return 0;
}
/** Main of program **/
int main( int argc, char **argv )
{
    int ret;
    int remote;                                 /* socket */
    int local_in;                               /* Local input */
    int local_out;                              /* Local output */
    int reason;
#ifdef _WIN32
    WSADATA wsadata;
    WSAStartup( 0x101, &wsadata);
#endif /* _WIN32 */

    /* initialization */
    yy_make_revstr();
    getarg( argc, argv );
    yy_debug("Program is $Revision: 100 $\n");

    /* Open local_in and local_out if forwarding a port */
    if ( local_type == LOCAL_SOCKET ) {
        /* Relay between local port and destination */
        local_in = local_out = accept_connection( local_port );
    } else {
        /* Relay between stdin/stdout and desteination */
        local_in = 0;
        local_out = 1;
#ifdef _WIN32
        _setmode(local_in, O_BINARY);
        _setmode(local_out, O_BINARY);
#endif
    }

retry:
#ifndef _WIN32
    if (0 < connect_timeout)
        set_timeout (connect_timeout);
#endif /* not _WIN32 */

    if (check_direct(dest_host))
        relay_method = METHOD_DIRECT;
    /* make connection */
    if ( relay_method == METHOD_DIRECT ) {
        remote = open_connection (dest_host, dest_port);
        if ( remote == SOCKET_ERROR )
            fatal( "Unable to connect to destination host, errno=%d\n",
                   socket_errno());
    } else {
        remote = open_connection (relay_host, relay_port);
        if ( remote == SOCKET_ERROR )
            fatal( "Unable to connect to relay host, errno=%d\n",
                   socket_errno());
    }

    /** resolve destination host (SOCKS) **/
#if !defined(_WIN32) && !defined(__CYGWIN32__)
    if (socks_ns.sin_addr.s_addr != 0)
        switch_ns (&socks_ns);
#endif /* not _WIN32 && not __CYGWIN32__ */
    if (relay_method == METHOD_SOCKS &&
        socks_resolve == RESOLVE_LOCAL &&
        local_resolve (dest_host, &dest_addr) < 0) {
        fatal("Unknown host: %s", dest_host);
    }

    /** relay negociation **/
    switch ( relay_method ) {
    case METHOD_SOCKS:
        if ( ((socks_version == 5) && (begin_socks5_relay(remote) < 0)) ||
             ((socks_version == 4) && (begin_socks4_relay(remote) < 0)) )
            fatal( "failed to begin relaying via SOCKS.\n");
        break;

    case METHOD_HTTP:
        ret = begin_http_relay(remote);
        switch (ret) {
        case START_ERROR:
            close (remote);
            fatal("failed to begin relaying via HTTP.\n");
        case START_OK:
            break;
        case START_RETRY:
            /* retry with authentication */
            close (remote);
            goto retry;
        }
        break;
    case METHOD_TELNET:
        if (begin_telnet_relay(remote) < 0)
             fatal("failed to begin relaying via telnet.\n");
        break;
    }
    debug("connected\n");

#ifndef _WIN32
    if (0 < connect_timeout)
        set_timeout (0);
#endif /* not _WIN32 */

    /* main loop */
    debug ("start relaying.\n");
do_repeater:
    reason = do_repeater(local_in, local_out, remote);
    debug ("relaying done.\n");
    if (local_type == LOCAL_SOCKET &&
        reason == REASON_CLOSED_BY_LOCAL &&
        f_hold_session) {
        /* re-wait at local port without closing remote session */
        debug ("re-waiting at local port %d\n", local_port);
        local_in = local_out = accept_connection( local_port );
        debug ("re-start relaying\n");
        goto do_repeater;
    }
    closesocket(remote);
    if ( local_type == LOCAL_SOCKET)
        closesocket(local_in);
#ifdef _WIN32
    WSACleanup();
#endif /* _WIN32 */
    yy_debug ("that's all, bye.\n");

    return 0;
}
