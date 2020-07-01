#include "dpc_io.h"

dpc_int dpc_read_byte(dpc_file f, dpc_byte * r) {
    if (1 == fread(r, sizeof(dpc_byte), 1, f)) {
        return 0;
    }
    return ferror(f);
}

dpc_int dpc_write_byte(dpc_file f, dpc_byte v) {
    if (1 == fwrite(&v, sizeof(dpc_byte), 1, f)) {
        return 0;
    }
    return ferror(f);
}

dpc_int dpc_read_short(dpc_file f, dpc_short * r) {
    if (1 == fread(r, sizeof(dpc_short), 1, f)) {
        *r = ntohs(*r);
        return 0;
    }
    return ferror(f);
}

dpc_int dpc_write_short(dpc_file f, dpc_short v) {
    v = htons(v);
    if (1 == fwrite(&v, sizeof(dpc_short), 1, f)) {
        return 0;
    }
    return ferror(f);
}

dpc_int dpc_read_int(dpc_file f, dpc_int * r) {
    if (1 == fread(r, sizeof(dpc_int), 1, f)) {
        *r = ntohl(*r);
        return 0;
    }
    return ferror(f);
}

dpc_int dpc_write_int(dpc_file f, dpc_int v) {
    v = htonl(v);
    if (1 == fwrite(&v, sizeof(dpc_int), 1, f)) {
        return 0;
    }
    return ferror(f);
}
