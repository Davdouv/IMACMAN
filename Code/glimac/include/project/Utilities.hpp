/* FICHIER - MAP


*/
#pragma once
#include <vector>

static bool isStaticElement(char e) {
    return !((e!='W') && (e!='G') && (e!='V') && (e!='S') && (e!='B')&& (e!='D') && (e!='X'));
}

static int charToInt(char c) {
    int e = 1 - '1';
    return c + e;
}

static int min(std::vector<int> tab) {
    
    if (tab.empty()) return -1;
    else {
        int min;
        min = tab[0];
        for (int i = 0; i < tab.size(); i++) {
            if (min > tab[i] && tab[i]!=-1) min = tab[i];
        }
        return min;
    }
}