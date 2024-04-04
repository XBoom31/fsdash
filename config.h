#pragma once

//
const int windowx = 1920;
const int windowy = 1080;
const float scale = 2.45;
const int fpscap = 60;
const bool fullscreen = true; // numa sa fie rezolutia buna si scaleu
//fonts
const int rpmfontsize = 40 * scale;
const int speedfontsize = 200 * scale;
const int tempfontsize = 30 * scale;    

//warning lights
const int light_padding = 10 * scale;
const int light_size = 25 * scale;
const char nume_iconite_warninguri[][20]{
    {"r2d"},
    {"bat"},
    {"overheat"},
    {"apps"},
    {"relay"},

};
const char path_iconite_warninguri[][100]{
    {"/home/relu/dev/xdash/readytodrive.png"},
    {"/home/relu/dev/xdash/bat.png"},
    {"/home/relu/dev/xdash/overheat.png"},
    {"/home/relu/dev/xdash/apps.png"},
    {"/home/relu/dev/xdash/relay.png"}
};
