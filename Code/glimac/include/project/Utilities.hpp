/* FICHIER - MAP


*/
#pragma once

static bool isStaticElement(char e) {
    return !((e!='W') && (e!='G') && (e!='V') && (e!='S') && (e!='B')&& (e!='D'));
}

static int charToInt(char c) {
    int e = 1 - '1';
    return c + e;
}
