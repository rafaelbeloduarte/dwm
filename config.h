/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "gaplessgrid.c"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const Gap default_gap        = {.isgap = 1, .realgap = 5, .gappx = 5};  
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=14" };
static const char dmenufont[]       = "monospace:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#0066CC";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class           instance    title             tags mask     switchtotag    isfloating   monitor */
	{ "Gimp",          NULL,       NULL,             0,            0,             1,           -1 },
	{ "Firefox",       NULL,       NULL,             1 << 8,       0,             0,           -1 },
	{ "Audacious",     NULL,       NULL,             1 << 3,       1,             0,           -1 },
	{ "Google-chrome", NULL,       NULL,             1 << 2,       1,             0,           -1 },
	{ "Steam",         NULL,       NULL,             1 << 6,       1,             0,           -1 },
	{ "mpv",           NULL,       NULL,             1 << 8,       0,             1,           -1 },
	{ "okular",        NULL,       NULL,             1 << 0,       1,             0,           -1 },
	{ NULL,            NULL,       "Friends List",   1 << 6,       1,             1,           -1 },
	{ NULL,            NULL,       "BashTOP",        1 << 8,       0,             1,           -1 },
	{ NULL,            NULL,       "mutt",           1 << 8,       0,             1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "###",      gaplessgrid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier           key                function        argument */
	{ MODKEY,             XK_space,          spawn,          {.v = dmenucmd } },
	{ MODKEY,             XK_Return,         spawn,          {.v = termcmd } },
	{ MODKEY,             XK_p,              togglebar,      {0} },
	{ MODKEY,             XK_j,              focusstack,     {.i = +1 } },
	{ MODKEY,             XK_k,              focusstack,     {.i = -1 } },
	{ MODKEY,             XK_i,              incnmaster,     {.i = +1 } },
	{ MODKEY,             XK_d,              incnmaster,     {.i = -1 } },
	{ MODKEY,             XK_h,              setmfact,       {.f = -0.05} },
	{ MODKEY,             XK_l,              setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,   XK_j,              movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,   XK_k,              movestack,      {.i = -1 } },
	{ MODKEY,             XK_z,              zoom,           {0} },			// move window to master
	{ MODKEY,             XK_Tab,            view,           {0} },			// move back to last used tag
	{ MODKEY,             XK_q,              killclient,     {0} },
	{ MODKEY,             XK_e,              setlayout,      {.v = &layouts[0]} },	// tiling layout
	{ MODKEY|ShiftMask,   XK_f,              setlayout,      {.v = &layouts[1]} },	// floating layout
	{ MODKEY,             XK_w,              setlayout,      {.v = &layouts[2]} },	// monocle layout (windowed)
	{ MODKEY,             XK_g,              setlayout,      {.v = &layouts[3] } },
	// { MODKEY,           XK_space,          setlayout,      {0} },
	{ ALTKEY,             XK_space,          togglefloating, {0} },
	{ MODKEY,             XK_0,              view,           {.ui = ~0 } },		// show all tags
	{ MODKEY|ShiftMask,   XK_0,              tag,            {.ui = ~0 } },		// pin window to all tags, use pin to specific tag to unpin from all
	{ MODKEY,             XK_comma,          focusmon,       {.i = -1 } },		// focusmon is used to focus monitor
	{ MODKEY,             XK_period,         focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,   XK_comma,          tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,   XK_period,         tagmon,         {.i = +1 } },
	{ MODKEY,             XK_minus,          setgaps,        {.i = -5 } },
	{ MODKEY,             XK_equal,          setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,   XK_minus,          setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,   XK_equal,          setgaps,        {.i = GAP_TOGGLE} },
	TAGKEYS(              XK_1,                              0)
	TAGKEYS(              XK_2,                              1)
	TAGKEYS(              XK_3,                              2)
	TAGKEYS(              XK_4,                              3)
	TAGKEYS(              XK_5,                              4)
	TAGKEYS(              XK_6,                              5)
	TAGKEYS(              XK_7,                              6)
	TAGKEYS(              XK_8,                              7)
	TAGKEYS(              XK_9,                              8)
	{ MODKEY|ShiftMask,   XK_e,              quit,           {0} },
	{ 0,                  XK_F11,            spawn,          SHCMD("amixer set Master 5%+")},
	{ 0,                  XK_F10,            spawn,          SHCMD("amixer set Master 5%-")},
	{ MODKEY,             XK_F4,             spawn,          SHCMD("amixer set Master toggle")},
	{ MODKEY,             XK_b,              spawn,          SHCMD("setxkbmap -layout br") },
	{ MODKEY,             XK_u,              spawn,          SHCMD("setxkbmap -layout us") },
	{ ALTKEY,             XK_l,              spawn,          SHCMD("sleep 1 && xset s activate && slock") },
	{ ALTKEY,             XK_e,              spawn,          SHCMD("keepassxc") },
	{ ALTKEY,             XK_f,              spawn,          SHCMD("firefox") },
	{ ALTKEY,             XK_b,              spawn,          SHCMD("qutebrowser") },
	{ ALTKEY,             XK_c,              spawn,          SHCMD("google-chrome-stable") },
	{ ALTKEY,             XK_a,              spawn,          SHCMD("pcmanfm")},
	{ ControlMask,        XK_space,          spawn,          SHCMD("alacritty -e ranger")},
	{ ALTKEY,             XK_m,              spawn,          SHCMD("alacritty -e bashtop")},
	{ ALTKEY,             XK_d,              spawn,          SHCMD("alacritty -t mutt -e mutt")},
	{ ALTKEY,             XK_s,              spawn,          SHCMD("steam")},
	{ ALTKEY,             XK_y,              spawn,          SHCMD("freetube")},
	{ ALTKEY,             XK_p,              spawn,          SHCMD("nitrogen --set-zoom-fill --random ~/.local/share/backgrounds/")},
	{ 0,                  XK_F7,             spawn,          SHCMD("audacious -t")},
	{ 0,                  XF86XK_AudioPrev,  spawn,          SHCMD("audacious -r")},
	{ 0,                  XF86XK_AudioNext,  spawn,          SHCMD("audacious -f")},
	{ ALTKEY|ShiftMask,   XK_f,              spawn,          SHCMD("chromium --proxy-server=\"proxy.uem.br:8080\"")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

