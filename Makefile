INCDIR=P-GP2/include
LIBDIR=P-GP2/lib
GP2_OBJECTS =
OBJECTS := ESN/*c util.c $(ea) $(main)
CC=gcc

CFLAGS = -I$(INCDIR) -L $(LIBDIR) -O2 -lgp2 -lgsl -lgslcblas -lm -g

default:	$(OBJECTS)
		rm -f -r pgp2;	mkdir pgp2;
		$(foreach var,$(GP2_OBJECTS),mkdir pgp2/$(var);	echo '$(var) compile'; P-GP2/bin/gp2 -l ../PGP2 -m $(var) -o pgp2/$(var) $(var).gp2; )
		$(CC) $(OBJECTS) $(foreach var,$(GP2_OBJECTS), pgp2/$(var)/*.c) $(CFLAGS) -o $(output)

clean:
		rm $(output)
		$(foreach var,$(GP2_OBJECTS),-rm -r $(var))
