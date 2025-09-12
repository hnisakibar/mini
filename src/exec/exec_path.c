#include "minishell.h"
#include "exec_int.h"

int     is_executable(const char *p)
{
    struct stat st;

    if (stat(p, &st) != 0)
        return (0);
    if (!S_ISREG(st.st_mode))
        return (0);
    return (access(p, X_OK) == 0);
}

char    *join3(const char *a, const char *b, const char *c)
{
    size_t  la;
    size_t  lb;
    size_t  lc;
    char    *s;

    la = ft_strlen(a);
    lb = ft_strlen(b);
    lc = ft_strlen(c);
    s = safe_malloc(la + lb + lc + 1);
    ft_memcpy(s, a, la);
    ft_memcpy(s + la, b, lb);
    ft_memcpy(s + la + lb, c, lc);
    s[la + lb + lc] = '\0';
    return (s);
}

// PATH dizgesinden bir segment kopyalanıp komutla birleştirilir
static char *build_candidate(const char *seg_start, size_t seg_len, const char *cmd)
{
	char	*dir_dup;
	char	*cand;

	if (seg_len == 0)
		dir_dup = ft_strdup(".");
	else
	{
		dir_dup = (char *)safe_malloc(seg_len + 1);
		ft_memcpy(dir_dup, seg_start, seg_len);
		dir_dup[seg_len] = '\0';
	}
	cand = join3(dir_dup, "/", cmd);
	free(dir_dup);
	return (cand);
}

// PATH içerisindeki dizinleri tarayıp çalıştırılabilir komut yolu döner
char *scan_path_dirs(char *paths, const char *cmd)
{
	const char	*seg_start = paths;
	const char	*seg_end;
	size_t		seg_len;
	char		*cand;

	while (1)
	{
		seg_end = ft_strchr(seg_start, ':');
		seg_len = (seg_end ? (size_t)(seg_end - seg_start) : ft_strlen(seg_start));
		cand = build_candidate(seg_start, seg_len, cmd);
		if (is_executable(cand))
			return (cand);
		free(cand);
		if (!seg_end)
			break;
		seg_start = seg_end + 1;
	}
	return (NULL);
}

char    *find_in_path(const char *cmd, t_env *env)
{
    const char  *path;
    char        *dup;
    char        *res;

    if (!cmd || !*cmd)
        return (NULL);
    if (ft_strchr(cmd, '/'))
        return (is_executable(cmd) ? ft_strdup(cmd) : NULL);
    path = env_get(env, "PATH");
    if (!path || !*path)
        return (NULL);
    dup = ft_strdup(path);
    if (!dup)
        return (NULL);
    res = scan_path_dirs(dup, cmd);
    free(dup);
    return (res);
}
