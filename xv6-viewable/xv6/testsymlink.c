#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

static void die(const char *m){ printf(1, "FAIL: %s\n", m); exit(); }
static void ok(const char *m){ printf(1, "PASS: %s\n", m); }

int main(void){
  int fd;
  char buf[64];

  //printf(1, "test 1\n");
  fd = open("tgt", O_CREATE | O_WRONLY);
  if(fd < 0) die("open tgt create");
  //printf(1, "here\n");
  if(write(fd, "hello\n", 6) != 6) die("write tgt");
  //printf(1, "here\n");
  close(fd);
  //printf(1, "here\n");
  if(symlink("tgt", "lnk") != 0) die("symlink tgt lnk");
  //printf(1, "here\n");
  ok("symlink created");
  
  //printf(1, "test 2\n");
  fd = open("lnk", O_RDONLY);
  //printf(1, "here\n");
  if(fd < 0) die("open lnk");
  int n = read(fd, buf, sizeof(buf)-1);
  if(n < 0) die("read lnk");
  buf[n] = 0;
  //printf(1,"here2\n");
  close(fd);
  //printf(1, "here3\n");
  if(strcmp(buf, "hello\n") != 0) die("content mismatch through symlink");
  ok("read through symlink");
  
  printf(1, "test 3\n");
  if(symlink("b", "a") != 0) die("symlink b a");
  if(symlink("a", "b") != 0) die("symlink a b");
  fd = open("a", O_RDONLY);
  if(fd >= 0){
    close(fd);
    die("expected open(a) to fail due to loop");
  }
  ok("loop detected (open failed)");

  printf(1, "testsymlink done\n");
  exit();
}
