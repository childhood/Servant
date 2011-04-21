/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "servant.h"

bool_t
xdr_chunk (XDR *xdrs, chunk *objp)
{
	register int32_t *buf;

	 if (!xdr_bytes (xdrs, (char **)&objp->chunk_val, (u_int *) &objp->chunk_len, CHUNK_LENGTH))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_servant_request (XDR *xdrs, servant_request *objp)
{
	register int32_t *buf;

	 if (!xdr_chunk (xdrs, &objp->data))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_servant_response (XDR *xdrs, servant_response *objp)
{
	register int32_t *buf;

	 if (!xdr_chunk (xdrs, &objp->data))
		 return FALSE;
	return TRUE;
}
