#include <ruby.h>
#include <stdio.h>
#include <sys/types.h>   /***********  Write Lock Setter  *******/
#include <unistd.h>      
#include <fcntl.h>

// LiteFS lock offsets
const int HaltByte = 72;

// Use Open File Decription Locks, if available
#ifndef F_OFD_SETLKW
#define F_OFD_SETLKW F_SETLKW
#endif

// Halt locks the HALT lock on the file handle to the LiteFS database lock file.
// This causes writes to be halted on the primary node so that this replica can
// perform writes until Unhalt() is invoked.
//
// The HALT lock will automatically be released when the file descriptor is closed.
//
// This function is a no-op if the current node is the primary.
static VALUE litefs_halt(VALUE self, VALUE filenum) {
  if (TYPE(filenum) != T_FIXNUM) {
    rb_raise(rb_eTypeError, "file number is required to be of type Fixnum");
    return Qnil;
  }

  int fd = FIX2INT(filenum);

  struct flock lock;
  lock.l_type    = F_WRLCK;
  lock.l_start   = HaltByte;
  lock.l_whence  = SEEK_SET;
  lock.l_len     = 1; 

  int rc = fcntl(fd, F_OFD_SETLKW, &lock);

  return INT2NUM(rc);
}

// Unhalt releases the HALT lock on the file handle to the LiteFS database lock
// file. This allows writes to resume on the primary node. This replica will
// not be able to perform any more writes until Halt() is called again.
//
// This function is a no-op if the current node is the primary or if the
// lock expired.
static VALUE litefs_unhalt(VALUE self, VALUE filenum) {
  if (TYPE(filenum) != T_FIXNUM) {
    rb_raise(rb_eTypeError, "file number is required to be of type Fixnum");
    return Qnil;
  }

  int fd = FIX2INT(filenum);

  struct flock lock;
  lock.l_type    = F_UNLCK;
  lock.l_start   = HaltByte;
  lock.l_whence  = SEEK_SET;
  lock.l_len     = 1; 

  int rc = fcntl(fd, F_OFD_SETLKW, &lock);

  return INT2NUM(rc);
}

// Attach methods to LiteFS module
void Init_litefs() {
  VALUE LiteFS = rb_const_get(rb_cObject, rb_intern("LiteFS"));

  rb_define_singleton_method(LiteFS, "halt", litefs_halt, 1);
  rb_define_singleton_method(LiteFS, "unhalt", litefs_unhalt, 1);
}
