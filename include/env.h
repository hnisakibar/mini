#ifndef ENV_H
# define ENV_H
typedef struct s_env {
    char *key;
    char *val;
    struct s_env *next;
} t_env;

t_env *env_init(char **environ);
char  *env_get(t_env *env, const char *key);
int    env_set(t_env **env, const char *key, const char *val, int overwrite);
int    env_unset(t_env **env, const char *key);
char **env_to_environ(t_env *env);
void   free_env(t_env *env);


void   env_normalize_startup(t_env **env);

/* internal env helpers used across env.c/env2.c */
t_env *new_var(const char *k, const char *v);
char  *make_pair(const char *k, const char *v);
t_env *create_env_node(char *entry);
#endif
