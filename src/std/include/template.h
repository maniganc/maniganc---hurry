
/* concatenate macro symbols */
#define TEMPLATE_CAT2(x, y) x##_##y
#define TEMPLATE_CAT3(x, y, z) x##_##y##_##z

/* evaluate macro symbols then concatenate */
#define TEMPLATE_EVAL_CAT2(x, y) TEMPLATE_CAT2(x, y)
#define TEMPLATE_EVAL_CAT3(x, y, z) TEMPLATE_CAT3(x, y, z)

/* class template */
#define CLASS_TEMPLATE(class, type) TEMPLATE_EVAL_CAT2(class, type)

/* class template method */
#define METHOD_TEMPLATE(class, type, m) TEMPLATE_EVAL_CAT3(class, type, m)

/* func template */
#define FUNC_TEMPLATE(func, type) TEMPLATE_EVAL_CAT2(func, type)
