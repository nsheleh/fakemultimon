#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <dlfcn.h>

#include <X11/extensions/Xinerama.h>

static XineramaScreenInfo *(*orig_XineramaQueryScreens)(Display *, int *);

static struct {
  int from;
  int to;
} mapping[10];

static int screen_count, mapping_count;

XineramaScreenInfo *XineramaQueryScreens(Display *display, int *screens) {
  XineramaScreenInfo *result = orig_XineramaQueryScreens(display, screens);
  if(!screen_count)
    return result;

  *screens = screen_count;
  for(int i=0;i<mapping_count;i++)
    memcpy(&result[mapping[i].to], &result[mapping[i].from], sizeof(XineramaScreenInfo));

  return result;
}

__attribute__ ((format (printf, 1, 2))) static void output(char *format, ...) {
  char buf[512];
  va_list va;

  va_start(va, format);
  vsnprintf(buf, sizeof(buf), format, va);
  va_end(va);

  fprintf(stderr, "fakemultimon: %s\n", buf);
  fflush(stderr);
}

static void usage(void) {
  output("usage:");
  output("  LD_PRELOAD=[path to .so file] MULTIMON_MAPPINGS=[number of screens],[mapping from (1)]:[mapping to (1)],[mapping from (2)]:[mapping to (2)],[etc]");
  output("example:");
  output("  MULTIMON_MAPPINGS=2,0:1,2:0");
  output("  sets number of screens to 2, and maps display 0 to 1, and 2 to 0");
}

__attribute__ ((constructor)) static void _init(void) {
  orig_XineramaQueryScreens = dlsym(RTLD_NEXT, "XineramaQueryScreens");

  char *mapping_env = getenv("MULTIMON_MAPPINGS");
  if(!mapping_env) {
    output("MULTIMON_MAPPINGS envvar not set.");
    usage();
    return;
  }

  int tokens = sscanf(mapping_env, "%d,%d:%d,%d:%d,%d:%d,%d:%d,%d:%d,%d:%d,%d:%d,%d:%d,%d:%d,%d:%d", &screen_count, &mapping[0].from, &mapping[0].to, &mapping[1].from, &mapping[1].to, &mapping[2].from, &mapping[2].to, &mapping[3].from, &mapping[3].to, &mapping[4].from, &mapping[4].to, &mapping[5].from, &mapping[5].to, &mapping[6].from, &mapping[6].to, &mapping[7].from, &mapping[7].to, &mapping[8].from, &mapping[8].to, &mapping[9].from, &mapping[9].to);
  if(tokens < 3 || tokens % 2 != 1) {
    output("invalid MULTIMON_MAPPINGS envvar.");
    usage();
    return;
  }

  mapping_count = (tokens - 1) / 2;

  output("%d screens, %d mappings:", screen_count, mapping_count);
  for(int i=0;i<mapping_count;i++)
    output("  %d -> %d", mapping[i].from, mapping[i].to);
}
