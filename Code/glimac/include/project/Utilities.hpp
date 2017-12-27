/* FICHIER - MAP


*/
#pragma once

static bool isStaticElement(char e) {
    return !((e!='W') && (e!='V') && (e!='G') && (e!='S') && (e!='P'));
}

static int charToInt(char c) {
    int e = 1 - '1';
    return c + e;
}
