#ifndef URLENCODE_H
#define URLENCODE_H

extern char rfc3986[256];;
extern char html5[256];

void url_encoder_rfc_tables_init();
char *url_encode( char *table, unsigned char *s, char *enc);

#endif