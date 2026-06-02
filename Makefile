SUBDIRS = ex1_hello_os ex2_trap ex3_psum ex4_coletivas ex5_vecadd ex6_derived

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done