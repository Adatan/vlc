/*****************************************************************************
 * vlc_tls.h: Transport Layer Security API
 *****************************************************************************
 * Copyright (C) 2004-2011 Rémi Denis-Courmont
 * Copyright (C) 2005-2006 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef VLC_TLS_H
# define VLC_TLS_H

/**
 * \file
 * This file defines Transport Layer Security API (TLS) in vlc
 */

# include <vlc_network.h>

typedef struct vlc_tls vlc_tls_t;
typedef struct vlc_tls_creds vlc_tls_creds_t;

/** TLS session */
struct vlc_tls
{
    VLC_COMMON_MEMBERS

    void *sys;
    int fd;

    struct virtual_socket_t sock;
};

VLC_API vlc_tls_t *vlc_tls_ClientSessionCreate (vlc_tls_creds_t *, int fd,
                                         const char *host, const char *service,
                                         const char *const *alpn, char **alp);
vlc_tls_t *vlc_tls_SessionCreate (vlc_tls_creds_t *, int fd, const char *host,
                                  const char *const *alpn);
int vlc_tls_SessionHandshake (vlc_tls_t *, const char *host, const char *serv,
                              char ** /*restrict*/ alp);
VLC_API void vlc_tls_SessionDelete (vlc_tls_t *);

VLC_API int vlc_tls_Read(vlc_tls_t *, void *buf, size_t len, bool waitall);
VLC_API char *vlc_tls_GetLine(vlc_tls_t *);
VLC_API int vlc_tls_Write(vlc_tls_t *, const void *buf, size_t len);

# define tls_Recv(a,b,c) vlc_tls_Read(a,b,c,false)
# define tls_Send(a,b,c) vlc_tls_Write(a,b,c)

/** TLS credentials (certificate, private and trust settings) */
struct vlc_tls_creds
{
    VLC_COMMON_MEMBERS

    module_t  *module;
    void *sys;

    int (*open) (vlc_tls_creds_t *, vlc_tls_t *, int fd, const char *host,
                 const char *const *alpn);
    int  (*handshake) (vlc_tls_t *, const char *host, const char *service,
                       char ** /*restrict*/ alp);
    void (*close) (vlc_tls_t *);
};

VLC_API vlc_tls_creds_t *vlc_tls_ClientCreate (vlc_object_t *);
vlc_tls_creds_t *vlc_tls_ServerCreate (vlc_object_t *,
                                       const char *cert, const char *key);
VLC_API void vlc_tls_Delete (vlc_tls_creds_t *);

#endif
