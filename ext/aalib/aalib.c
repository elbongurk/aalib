#include <ruby.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aalib.h"
#include "aaint.h"

struct aa_hardware_params aa_defparams =
{
    /*&aa_font16*/NULL, AA_NORMAL_MASK | AA_DIM_MASK | AA_BOLD_MASK
};
void aa_uninitmouse(struct aa_context *c)
{
    if (c->mousedriver != NULL) {
	c->mousedriver->uninit(c);
        if(c->mousedriverdata) free(c->mousedriverdata);
	c->mousedriverdata=NULL;
	c->mousedriver = NULL;
	c->mousemode=0;
    }
}
void aa_uninitkbd(struct aa_context *c)
{
    if (c->kbddriver != NULL) {
	if (c->mousedriver != NULL)
	    aa_uninitmouse(c);
	c->mousedriverdata=NULL;
	c->kbddriver->uninit(c);
        if(c->kbddriverdata) free(c->kbddriverdata);
	c->kbddriverdata=NULL;
	c->kbddriver = NULL;
    }
}

int aa_resize(aa_context * c)
{
    int width, height;
    width = abs(c->params.width);
    height = abs(c->params.height);
    c->driver->getsize(c, &width, &height);
    if (width <= 0 || height <= 0) {
	printf("Invalid buffer sizes!\n");
	exit(-1);
    }
    if (width != aa_scrwidth(c) || height != aa_imgheight(c)) {
	if (c->imagebuffer != NULL)
	    free(c->imagebuffer);
	if (c->textbuffer != NULL)
	    free(c->textbuffer);
	if (c->attrbuffer != NULL)
	    free(c->attrbuffer);
	c->params.width = width;
	c->params.height = height;
	c->imgwidth = width * c->mulx;
	c->imgheight = height * c->mulx;
	if ((c->imagebuffer = calloc(1, c->imgwidth * c->imgheight)) == NULL)
	    return 0;
	if ((c->textbuffer = calloc(1, aa_scrwidth(c) * aa_scrheight(c))) == NULL) {
	    free(c->imagebuffer);
	    return 0;
	}
	memset(c->textbuffer, ' ', c->params.width * c->params.height);
	if ((c->attrbuffer = calloc(1, c->params.width * c->params.height)) == NULL) {
	    free(c->imagebuffer);
	    free(c->textbuffer);
	    return 0;
	}
    }
    if (!c->driverparams.mmwidth)
	c->params.mmwidth = 290;
    else
	c->params.mmwidth = c->driverparams.mmwidth;
    if (!c->driverparams.mmheight)
	c->params.mmheight = 215;
    else
	c->params.mmheight = c->driverparams.mmheight;
    if (!c->driverparams.minwidth)
	c->params.minwidth = c->params.width;
    else
	c->params.minwidth = c->driverparams.minwidth;
    if (!c->driverparams.minheight)
	c->params.minheight = c->params.height;
    else
	c->params.minheight = c->driverparams.minheight;
    if (!c->driverparams.maxwidth)
	c->params.maxwidth = c->params.width;
    else
	c->params.maxwidth = c->driverparams.maxwidth;
    if (!c->driverparams.maxheight)
	c->params.maxheight = c->params.height;
    else
	c->params.maxheight = c->driverparams.maxheight;
    return 1;
}
aa_context *aa_init(__AA_CONST struct aa_driver * driver, __AA_CONST struct aa_hardware_params * defparams, __AA_CONST void *driverdata)
{
    struct aa_context *c;
    c = calloc(1, sizeof(*c));
    c->driverdata=NULL;
    c->mousedriverdata=NULL;
    c->kbddriverdata=NULL;
    if (c == NULL) {
	return NULL;
    }
    if (!driver->init(defparams, driverdata,&c->driverparams,&c->driverdata)) {
        free(c);
	return NULL;
    }
    c->driver = driver;
    c->kbddriver = NULL;
    c->mousedriver = NULL;
    c->params.supported = c->driverparams.supported & defparams->supported;
    if (defparams->font)
    {
	c->params.font = defparams->font;
    }
    else
        c->params.font = c->driverparams.font;
    if (!c->params.font)
	c->params.font = defparams->font;
    if (!c->params.font)
	c->params.font = &aa_font16;
    if (!c->params.supported)
	c->params.supported = c->driverparams.supported;
    c->mulx = 2;
    c->muly = 2;
    c->cursorx = 0;
    c->cursory = 0;
    c->mousex = 0;
    c->mousey = 0;
    c->buttons = 0;
    c->table = NULL;
    c->filltable = NULL;
    c->parameters = NULL;
    if (defparams->width)
	c->params.width = defparams->width;
    else if (c->driverparams.width)
	c->params.width = c->driverparams.width;
    else if (defparams->recwidth)
	c->params.recwidth = defparams->recwidth;
    else if (c->driverparams.recwidth)
	c->params.recwidth = c->driverparams.recwidth;
    else
	c->params.width = 80;
    if (defparams->minwidth > c->params.width)
	c->params.width = defparams->minwidth;
    if (c->driverparams.minwidth > c->params.width)
	c->params.width = c->driverparams.minwidth;
    if (defparams->maxwidth && defparams->maxwidth > c->params.width)
	c->params.width = defparams->maxwidth;
    if (c->driverparams.maxwidth && c->driverparams.maxwidth > c->params.width)
	c->params.width = c->driverparams.maxwidth;

    if (defparams->height)
	c->params.height = defparams->height;
    else if (c->driverparams.height)
	c->params.height = c->driverparams.height;
    else if (defparams->recheight)
	c->params.recheight = defparams->recheight;
    else if (c->driverparams.recheight)
	c->params.recheight = c->driverparams.recheight;
    else
	c->params.height = 25;
    if (defparams->minheight > c->params.height)
	c->params.height = defparams->minheight;
    if (c->driverparams.minheight > c->params.height)
	c->params.height = c->driverparams.minheight;
    if (defparams->maxheight && defparams->maxheight > c->params.height)
	c->params.height = defparams->maxheight;
    if (c->driverparams.maxheight && c->driverparams.maxheight > c->params.height)
	c->params.height = c->driverparams.maxheight;
    c->params.width *= -1;
    c->params.height *= -1;
    c->params.dimmul = 5.3;
    c->params.boldmul = 2.7;
    if(c->driverparams.dimmul) c->params.dimmul=c->driverparams.dimmul;
    if(c->driverparams.boldmul) c->params.boldmul=c->driverparams.boldmul;
    if(defparams->dimmul) c->params.dimmul=defparams->dimmul;
    if(defparams->boldmul) c->params.boldmul=defparams->boldmul;
    c->imagebuffer = NULL;
    c->textbuffer = NULL;
    c->attrbuffer = NULL;
    c->resizehandler = NULL;
    if (!aa_resize(c)) {
	driver->uninit(c);
	if(c->driverdata!=NULL) free(c->driverdata);
	free(c);
	printf("out of memory\n");
	return NULL;
    }
    if (!aa_validmode(aa_scrwidth(c), aa_scrheight(c), defparams)) {
	aa_close(c);
	return 0;
    }
    return (c);
}
static void aa_invalidate(aa_context * c)
{
    if (c->table != NULL)
	free(c->table);
    if (c->filltable != NULL)
	free(c->filltable);
    if (c->parameters != NULL)
	free(c->parameters);
    c->table = NULL;
    c->filltable = NULL;
    c->parameters = NULL;
}
void aa_setfont(aa_context * c, __AA_CONST struct aa_font *font)
{
    c->params.font = font;
    aa_invalidate(c);
}
void aa_setsupported(aa_context * c, int supported)
{
    c->params.supported = c->driverparams.supported & supported;
    if (!c->params.supported)
	c->params.supported = c->driverparams.supported;
    aa_invalidate(c);
}
void aa_close(aa_context * c)
{
    if (c->cursorstate < 0 && c->driver->cursormode)
	c->driver->cursormode(c, 1);
    if (c->kbddriver != NULL)
	aa_uninitkbd(c);
    c->driver->uninit(c);
    aa_invalidate(c);
    if (c->imagebuffer != NULL)
	free(c->imagebuffer);
    if (c->textbuffer != NULL)
	free(c->textbuffer);
    if (c->attrbuffer != NULL)
	free(c->attrbuffer);
    if(c->driverdata) free(c->driverdata);
    free(c);
}

//BEGIN CRUBY

static VALUE aarb_init(VALUE self) {
  return self;
}

static VALUE aarb_imgwidth(VALUE self) {
  aa_context *ptr;
  Data_Get_Struct(self, aa_context, ptr);
  return INT2FIX(aa_imgwidth(ptr));
}

static VALUE aarb_scrwidth(VALUE self) {
  aa_context *ptr;
  Data_Get_Struct(self, aa_context, ptr);
  return INT2FIX(aa_scrwidth(ptr));
}

static VALUE aarb_imgheight(VALUE self) {
  aa_context *ptr;
  Data_Get_Struct(self, aa_context, ptr);
  return INT2FIX(aa_imgheight(ptr));
}

static VALUE aarb_scrheight(VALUE self) {
  aa_context *ptr;
  Data_Get_Struct(self, aa_context, ptr);
  return INT2FIX(aa_scrheight(ptr));
}

static VALUE aarb_putpixel(VALUE self, VALUE x, VALUE y, VALUE color) {
  aa_context *ptr;
  Data_Get_Struct(self, aa_context, ptr);  
  aa_putpixel(ptr, NUM2INT(x), NUM2INT(y), NUM2INT(color));
  return Qnil;
}

static VALUE aarb_getpixel(VALUE self, VALUE x, VALUE y) {
  aa_context *ptr;
  Data_Get_Struct(self, aa_context, ptr);
  return CHR2FIX(ptr->textbuffer[NUM2INT(x) + NUM2INT(y) * aa_scrwidth(ptr)]);
}

static VALUE aarb_getattr(VALUE self, VALUE x, VALUE y) {
  aa_context *ptr;
  Data_Get_Struct(self, aa_context, ptr);
  return CHR2FIX(ptr->attrbuffer[NUM2INT(x) + NUM2INT(y) * aa_scrwidth(ptr)]);
}

static VALUE aarb_getcolor(VALUE self, VALUE x, VALUE y) {
  aa_context *ptr;
  Data_Get_Struct(self, aa_context, ptr);

  int imgpos = 4 * NUM2INT(x) + 2 * NUM2INT(y) * aa_imgwidth(ptr);

  return INT2FIX((ptr->imagebuffer[imgpos] + 
                  ptr->imagebuffer[imgpos+1] + 
                  ptr->imagebuffer[imgpos+aa_imgwidth(ptr)] + 
                  ptr->imagebuffer[imgpos+aa_imgwidth(ptr)+1]) / 4);
}

static VALUE aarb_render(int argc, VALUE* argv, VALUE self) {
  VALUE temp;
  aa_context *ptr;

  Data_Get_Struct(self, aa_context, ptr);

  //Brightness in range 0 (normal) to 255 (white)
  //Contrast value in range 0 (normal) to 127 (white)
  //Gamma value in the standard range
  //Dithering algorithm AA_NONE, AA_ERRORDISTRIB, AA_FLOYD_S, AA_DITHERTYPES
  //Inversed terminals use 1 (black on white)
  //Range of random value added to each pixel for random dithering

  aa_renderparams rp = {0, 0, 1.0, AA_FLOYD_S, 1, 0};

  if (argc == 1) {
    temp = rb_hash_aref(argv[0], ID2SYM(rb_intern("bright")));
    if (temp != Qnil) {
      rp.bright = FIX2INT(temp);
    }
    temp = rb_hash_aref(argv[0], ID2SYM(rb_intern("contrast")));
    if (temp != Qnil) {
      rp.contrast = FIX2INT(temp);
    }
    temp = rb_hash_aref(argv[0], ID2SYM(rb_intern("gamma")));
    if (temp != Qnil) {
      rp.gamma = (float)NUM2DBL(temp);
    }
    temp = rb_hash_aref(argv[0], ID2SYM(rb_intern("dither")));
    if (temp != Qnil) {
      rp.dither = FIX2INT(temp);
    }
    temp = rb_hash_aref(argv[0], ID2SYM(rb_intern("random")));
    if (temp != Qnil) {
      rp.randomval = FIX2INT(temp);
    }
    temp = rb_hash_aref(argv[0], ID2SYM(rb_intern("inverse")));
    if (temp != Qnil) {
      if (temp == Qtrue) {
        rp.inversion = 1;
      }
      else {
        rp.inversion = 0;
      }
    }
  }

  aa_render(ptr, &rp, 0, 0, aa_scrwidth(ptr), aa_scrheight(ptr));

  aa_flush(ptr);

  return Qnil;
}

static void aarb_free(void *ptr) {
  aa_close(ptr);
}

static VALUE aarb_new(VALUE class, VALUE width, VALUE height) {
  aa_hardwareparams hp = {
    /*&aa_font16*/NULL,
    AA_DIM_MASK | AA_NORMAL_MASK | AA_BOLD_MASK | AA_BOLDFONT_MASK
  };

  hp.width = NUM2INT(width) / 2;
  hp.height = NUM2INT(height) / 2;

  aa_context *ptr = aa_init(&mem_d, &hp, NULL);

  VALUE aa = Data_Wrap_Struct(class, 0, aarb_free, ptr);

  rb_obj_call_init(aa, 0, 0);
  return aa;
}

VALUE mAAlib;
void Init_aalib(void) {
  mAAlib = rb_define_module("AAlib");
  
  VALUE cAAlibContext = rb_define_class_under(mAAlib, "Context", rb_cObject);

  rb_define_singleton_method(cAAlibContext, "new", aarb_new, 2);
  rb_define_method(cAAlibContext, "initialize", aarb_init, 0);
  rb_define_method(cAAlibContext, "imgwidth", aarb_imgwidth, 0);
  rb_define_method(cAAlibContext, "imgheight", aarb_imgheight, 0);
  rb_define_method(cAAlibContext, "scrwidth", aarb_scrwidth, 0);
  rb_define_method(cAAlibContext, "scrheight", aarb_scrheight, 0);
  rb_define_method(cAAlibContext, "putpixel", aarb_putpixel, 3);
  rb_define_method(cAAlibContext, "getpixel", aarb_getpixel, 2);
  rb_define_method(cAAlibContext, "getattr", aarb_getattr, 2);
  rb_define_method(cAAlibContext, "getcolor", aarb_getcolor, 2);
  rb_define_method(cAAlibContext, "render", aarb_render, -1);
}
