# compilation tip
# disabling otimization (-O0)
# standard of C99 (-std=c99)

all:
	gcc test_mmap.c -g -O0 -o test_mmap
	gcc test_posix_memalign.c -g -O0 -o test_posix_memalign

show-warnings:
	gcc -Q --help=warning | sed -e 's/^\s*\(\-\S*\)\s*\[\w*\]/\1 /gp;d' | tr -d '\n'

# mmap tests
run-mmap:
	./test_mmap $(MEMSIZE)

mmap-perf-sequential:
	perf stat -B -e LLC-store-misses,LLC-load-misses ./test_mmap $(MEMSIZE) 0 ${FILEPATH} 2> mmap-perf-sequential-$(MEMSIZE)mb.out

mmap-perf-aleatory:
	perf stat -B -e LLC-store-misses,LLC-load-misses ./test_mmap $(MEMSIZE) 1 $(FILEPATH) 2> mmap-perf-aleatory-$(MEMSIZE)mb.out

mmap-perf: mmap-perf-sequential mmap-perf-aleatory

mmap-valgrind-sequential:
	valgrind --tool=lackey --trace-mem=yes ./test_mmap $(MEMSIZE) 0 2> valgrind-sequential-$(MEMSIZE)mb.out

mmap-valgrind-aleatory:
	valgrind --tool=lackey --trace-mem=yes ./test_mmap $(MEMSIZE) 1 2> valgrind-aleatory-$(MEMSIZE)mb.out

mmap-valgrind-sequential-cache:
	valgrind --tool=cachegrind ./test_mmap $(MEMSIZE) 0 2> valgrind-sequential-cache-$(MEMSIZE)mb.out

mmap-valgrind-aleatory-cache:
	valgrind --tool=cachegrind ./test_mmap $(MEMSIZE) 1 2> valgrind-aleatory-cache-$(MEMSIZE)mb.out

mmap-valgrind: mmap-valgrind-sequential mmap-valgrind-aleatory

# posix_memalign tests
posix-memalign-run:
	./test_posix_memalign $(MEMSIZE)

posix-memalign-perf-sequential:
	perf stat -B -e LLC-store-misses,LLC-load-misses ./test_posix_memalign $(MEMSIZE) 0 2> posix-memalign-perf-sequential-$(MEMSIZE)mb.out

posix-memalign-perf-aleatory:
	perf stat -B -e LLC-store-misses,LLC-load-misses ./test_posix_memalign $(MEMSIZE) 1 2> posix-memalign-perf-aleatory-$(MEMSIZE)mb.out

posix-memalign-perf: posix-memalign-perf-sequential posix-memalign-perf-aleatory

posix-memalign-valgrind-sequential:
	valgrind --tool=lackey --trace-mem=yes ./test_posix_memalign $(MEMSIZE) 0 2> valgrind-sequential-$(MEMSIZE)mb.out

posix-memalign-valgrind-aleatory:
	valgrind --tool=lackey --trace-mem=yes ./test_posix_memalign $(MEMSIZE) 1 2> valgrind-aleatory-$(MEMSIZE)mb.out

posix-memalign-valgrind-sequential-cache:
	valgrind --tool=cachegrind ./test_posix_memalign $(MEMSIZE) 0 2> valgrind-sequential-cache-$(MEMSIZE)mb.out

posix-memalign-valgrind-aleatory-cache:
	valgrind --tool=cachegrind ./test_posix_memalign $(MEMSIZE) 1 2> valgrind-aleatory-cache-$(MEMSIZE)mb.out

posix-memalign-valgrind: posix-memalign-valgrind-sequential posix-memalign-valgrind-aleatory

