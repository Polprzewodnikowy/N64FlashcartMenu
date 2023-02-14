#ifndef UTILS_H__
#define UTILS_H__


#define	ALIGN(x, a)     (((x) + ((typeof(x))(a) - 1)) & ~((typeof(x))(a) - 1))

#define MAX(a,b)        ({ typeof(a) _a = a; typeof(b) _b = b; _a > _b ? _a : _b; })
#define MIN(a,b)        ({ typeof(a) _a = a; typeof(b) _b = b; _a < _b ? _a : _b; })

#define KiB(x)          ((x) * 1024)
#define MiB(x)          ((x) * 1024 * 1024)


#endif
