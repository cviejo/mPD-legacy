/* Copyright (c) 1997-1999 Miller Puckette.
* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

#ifdef HAVE_LIBDL
#include <dlfcn.h>
#endif
#ifdef UNISTD
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#endif
#ifdef MSW
#include <io.h>
#include <windows.h>
#endif
#ifdef __APPLE__
#include <mach-o/dyld.h> 
#endif
#include <string.h>
#include "m_pd.h"
#include "s_stuff.h"
#include <stdio.h>
#include <sys/stat.h>
#ifdef _MSC_VER  /* This is only for Microsoft's compiler, not cygwin, e.g. */
#define snprintf sprintf_s
#endif

typedef void (*t_xxx)(void);

/* naming convention for externs.  The names are kept distinct for those
who wich to make "fat" externs compiled for many platforms.  Less specific
fallbacks are provided, primarily for back-compatibility; these suffice if
you are building a package which will run with a single set of compiled
objects.  The specific name is the letter b, l, d, or m for  BSD, linux,
darwin, or microsoft, followed by a more specific string, either "fat" for
a fat binary or an indication of the instruction set. */

#ifdef __FreeBSD__
static char sys_dllextent[] = ".b_i386", sys_dllextent2[] = ".pd_freebsd";
#elif defined(__linux__) || defined(__FreeBSD_kernel__) || defined(__GNU__)
static char sys_dllextent2[] = ".pd_linux";
# ifdef __x86_64__
static char sys_dllextent[] = ".l_ia64"; // this should be .l_x86_64 or .l_amd64
# elif defined(__i386__) || defined(_M_IX86)
static char sys_dllextent[] = ".l_i386";
# elif defined(__arm__)
static char sys_dllextent[] = ".l_arm";
# else
static char sys_dllextent[] = ".so";
# endif
#elif defined(__APPLE__)
# ifndef MACOSX3
static char sys_dllextent[] = ".d_fat", sys_dllextent2[] = ".pd_darwin";
# else
static char sys_dllextent[] = ".d_ppc", sys_dllextent2[] = ".pd_darwin";
# endif
#elif defined(_WIN32) || defined(__CYGWIN__)
static char sys_dllextent[] = ".m_i386", sys_dllextent2[] = ".dll";
#else
static char sys_dllextent[] = ".so", sys_dllextent2[] = ".so";
#endif

    /* maintain list of loaded modules to avoid repeating loads */
typedef struct _loadedlist
{
    struct _loadedlist *ll_next;
    t_symbol *ll_name;
} t_loadlist;

static t_loadlist *sys_loaded;
int sys_onloadlist(char *classname) /* return true if already loaded */
{
    t_symbol *s = gensym(classname);
    t_loadlist *ll;
    for (ll = sys_loaded; ll; ll = ll->ll_next)
        if (ll->ll_name == s)
            return (1);
    return (0);
}

void sys_putonloadlist(char *classname) /* add to list of loaded modules */
{
    t_loadlist *ll = (t_loadlist *)getbytes(sizeof(*ll));
    ll->ll_name = gensym(classname);
    ll->ll_next = sys_loaded;
    sys_loaded = ll;
    /* post("put on list %s", classname); */
}

void class_set_extern_dir(t_symbol *s);

static int sys_do_load_lib(t_canvas *canvas, char *objectname)
{
	return (1);
}

/* linked list of loaders */
typedef struct loader_queue {
    loader_t loader;
    struct loader_queue *next;
} loader_queue_t;

static loader_queue_t loaders = {sys_do_load_lib, NULL};

/* register class loader function */
void sys_register_loader(loader_t loader)
{
    loader_queue_t *q = &loaders;
    while (1)
    {
        if (q->next) 
            q = q->next;
        else
        {
            q->next = (loader_queue_t *)getbytes(sizeof(loader_queue_t));
            q->next->loader = loader;
            q->next->next = NULL;
            break;
        }
    }   
}

int sys_load_lib(t_canvas *canvas, char *classname)
{
    int dspstate = canvas_suspend_dsp();
    int ok = 0;
    loader_queue_t *q;
    for(q = &loaders; q; q = q->next)
        if (ok = q->loader(canvas, classname)) break;
    canvas_resume_dsp(dspstate);
    return ok;
}

int sys_run_scheduler(const char *externalschedlibname,
    const char *sys_extraflagsstring)
{
	 return (1);
}
