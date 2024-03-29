/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 2; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */
static const unsigned int gappih = 8;   /* horiz inner gap between windows */
static const unsigned int gappiv = 8;   /* vert inner gap between windows */
static const unsigned int gappoh =
    8; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov =
    8; /* vert outer gap between windows and screen edge */
static const int smartgaps =
    1; /* 1 means no outer gap when there is only one window */
static const int swallowfloating =
    1; /* 1 means swallow floating windows by default */
static const unsigned int systraypinning =
    0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor
          X */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const int systraypinningfailfirst =
    1; /* 1: if pinning fails, display systray on the first monitor, False:
          display systray on the last monitor*/
static const int showsystray = 1; /* 0 means no systray */
static const int showbar = 1;     /* 0 means no bar */
static const int topbar = 1;      /* 0 means bottom bar */
static const char *fonts[] = {
    "Hack Nerd Font:size=12",
    "JoyPixels:pixelsize=10:antialias=true:autohint=true"};
static const char dmenufont[] = "Hack Neard Font:size=12";
static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#eeeeee";
static const char col_cyan[] = "#005577";
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
    [SchemeSel] = {col_gray4, col_cyan, col_cyan},
};

typedef struct {
  const char *name;
  const void *cmd;
} Sp;
const char *spcmd1[] = {"alacritty", "--class", "spterm", NULL};
const char *spcmd2[] = {"alacritty", "--class", "spranger",
                        "-e",        "ranger",  NULL};
/* const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger",
 * NULL }; */
static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm", spcmd1},
    {"spranger", spcmd2},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class     instance  title           tags mask  isfloating  isterminal
       noswallow  monitor */
    {"Gimp", NULL, NULL, 0, 1, 0, 0, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, 0, -1, -1},
    {"st", NULL, NULL, 0, 0, 1, -1, -1},
    {"Alacritty", NULL, NULL, 0, 0, 1, -1, -1},
    {NULL, NULL, "Event Tester", 0, 1, 0, 1, -1},    /* xev */
    {NULL, "spterm", NULL, SPTAG(0), 1, 0, 1, -1},   /* xev */
    {NULL, "spranger", NULL, SPTAG(1), 1, 0, 1, -1}, /* xev */
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
    {"[D]", deck},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
    "dmenu_run", "-m",      dmenumon, "-fn",    dmenufont, "-nb",     col_gray1,
    "-nf",       col_gray3, "-sb",    col_cyan, "-sf",     col_gray4, NULL};
static const char *termcmd[] = {"alacritty", NULL};
static const char *roficmd[] = {"rofi", "-show", "combi"};

static Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_e, focusstack, {.i = +1}},
    {MODKEY, XK_u, focusstack, {.i = -1}},
    {MODKEY | ShiftMask, XK_e, pushup, {0}},
    {MODKEY | ShiftMask, XK_u, pushdown, {0}},
    /* { MODKEY,                       XK_s,      swapfocus,      {.i = -1 } },
     */
    {MODKEY | ControlMask, XK_o, incnmaster, {.i = +1}},
    {MODKEY | ShiftMask, XK_o, incnmaster, {.i = -1}},
    {MODKEY, XK_n, setmfact, {.f = -0.05}},
    {MODKEY, XK_i, setmfact, {.f = +0.05}},
    {MODKEY, XK_space, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY, XK_q, killclient, {0}},
    {MODKEY, XK_1, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_2, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_3, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_4, setlayout, {.v = &layouts[3]}},
    {MODKEY | ShiftMask, XK_Return, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    {MODKEY, XK_y, togglescratch, {.ui = 0}},
    {MODKEY, XK_l, togglescratch, {.ui = 1}},
    TAGKEYS(XK_a, 0) TAGKEYS(XK_r, 1) TAGKEYS(XK_s, 2) TAGKEYS(XK_t, 3)
        TAGKEYS(XK_d, 4) TAGKEYS(XK_z, 5) TAGKEYS(XK_x, 6) TAGKEYS(XK_v, 7)
            TAGKEYS(XK_c, 8){MODKEY | ShiftMask, XK_q, quit, {0}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function argument
     */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button1, sigdwmblocks, {.i = 1}},
    {ClkStatusText, 0, Button2, sigdwmblocks, {.i = 2}},
    {ClkStatusText, 0, Button3, sigdwmblocks, {.i = 3}},
    {ClkStatusText, 0, Button4, sigdwmblocks, {.i = 4}},
    {ClkStatusText, 0, Button5, sigdwmblocks, {.i = 5}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button1, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
