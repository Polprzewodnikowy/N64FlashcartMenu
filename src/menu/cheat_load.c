#include <string.h>

#include <libdragon.h>

#include <stdio.h>
#include <sys/stat.h>
#include "views/views.h"
#include "cheat_load.h"
#include "../utils/fs.h"


char *cheat_load_convert_error_message (cheat_load_err_t err) {
    switch (err) {
        case CHEAT_LOAD_OK: return "Cart load OK";
        case CHEAT_LOAD_ERR_NO_CHEAT_FILE: return "No cheat file found";
        case CHEAT_LOAD_ERR_SIZE_FAILED: return "Error occured acquiring cheat size";
        case CHEAT_LOAD_ERR_CHEAT_EMPTY: return "Cheat file is empty";
        case CHEAT_LOAD_ERR_CHEAT_TOO_LARGE: return "Cheat file is too large (over 128KiB)";
        case CHEAT_LOAD_ERR_MALLOC_FAILED: return "Error occured allocating memory for file";
        case CHEAT_LOAD_ERR_READ_FAILED: return "Error occured during file read";
        case CHEAT_LOAD_ERR_CLOSE_FAILED: return "Error occured during file close";
        default: return "Unknown error [CHEAT_LOAD]";
    }
}

/*
static size_t   strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (!dst || !src)
		return (0);
	if (size > 0)
	{
		while (--size && src[i])
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	while (src[i])
		i++;
	return (i);
}
*/
static int	find_str(char const *s, char c)
{
	int	i;
	int	nb_str;

	i = 0;
	nb_str = 0;
	if (!s[0])
		return (0);
	while (s[i] && s[i] == c)
		i++;
	while (s[i])
	{
		if (s[i] == c)
		{
			nb_str++;
			while (s[i] && s[i] == c)
				i++;
			continue ;
		}
		i++;
	}
	if (s[i - 1] != c)
		nb_str++;
	return (nb_str);
}

static void	get_next_str(char **next_str, size_t *next_strlen, char c)
{
	size_t	i;

	*next_str += *next_strlen;
	*next_strlen = 0;
	i = 0;
	while (**next_str && **next_str == c)
		(*next_str)++;
	while ((*next_str)[i])
	{
		if ((*next_str)[i] == c)
			return ;
		(*next_strlen)++;
		i++;
	}
}

static char	**free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	char	*next_str;
	size_t	next_strlen;
	int		i;

	i = -1;
	if (!s)
		return (NULL);
	tab = malloc(sizeof(char *) * (find_str(s, c) + 1));
	if (!tab)
		return (NULL);
	next_str = (char *)s;
	next_strlen = 0;
	while (++i < find_str(s, c))
	{
		get_next_str(&next_str, &next_strlen, c);
		tab[i] = (char *)malloc(sizeof(char) * (next_strlen + 1));
		if (!tab[i])
			return (free_tab(tab));
		strlcpy(tab[i], next_str, next_strlen + 1);
	}
	tab[i] = NULL;
	return (tab);
}

cheat_load_err_t load_cheats(menu_t *menu) {
    FILE *cheatsFile;
    struct stat st;
    size_t cheatsLength;
	path_t *path = path_clone(menu->load.rom_path);

    //Parse cheats from file
    path_ext_replace(path, "cht");
    if((cheatsFile = fopen(path_get(path), "rb")) == NULL) {
        //menu_show_error(menu, "Could not open cheat file.");
		path_free(path);
        return CHEAT_LOAD_OK;//no file is not an error. //CHEAT_LOAD_ERR_NO_CHEAT_FILE;
    }

    if (fstat(fileno(cheatsFile), &st)){
        //menu_show_error(menu, "Could not get cheat file size.");
		path_free(path);
        return CHEAT_LOAD_ERR_SIZE_FAILED;
    }

    cheatsLength = st.st_size;
    if (cheatsLength <= 0) {
       // menu_show_error(menu, "Cheat file is empty");
	   path_free(path);
        return CHEAT_LOAD_ERR_CHEAT_EMPTY;
    }
    if (cheatsLength > KiB(128)) {
        //menu_show_error(menu, "Cheat file is too large");
		path_free(path);
        return CHEAT_LOAD_ERR_CHEAT_TOO_LARGE;
    }

    char *cheatsContent = NULL;
    if((cheatsContent = malloc((cheatsLength + 1) * sizeof(char))) == NULL) {
        //menu_show_error(menu, "Couldnt allocate memory for file.");
		path_free(path);
        return CHEAT_LOAD_ERR_MALLOC_FAILED;
    }
    if(fread(cheatsContent, cheatsLength, 1, cheatsFile) != 1) {
       // menu_show_error(menu, "Could not read cheat file.");
	   	path_free(path);
        return CHEAT_LOAD_ERR_READ_FAILED;
    }

    cheatsContent[cheatsLength] = '\0';
    if(fclose(cheatsFile) != 0){
        //menu_show_error(menu, "Could not close cheat file.");
		path_free(path);
        return CHEAT_LOAD_ERR_CLOSE_FAILED;
    }
    cheatsFile = NULL;
 
    char **tab = ft_split(cheatsContent, '\n');
    //if (sizeof(tab) < 1 || sizeof(tab) > 3) {
       // menu_show_error(menu, "Cheat file split failed");
    //    return FLASHCART_ERR_FUNCTION_NOT_SUPPORTED;
    //}
    free(cheatsContent);
    //should have good tab here
    //we will assume line number = total cheat size. doesnt really matter
    uint32_t  *cheats = (uint32_t*)malloc(((sizeof(tab) * sizeof(uint32_t)) * 2) + 2);
	memset(cheats, 0, sizeof(tab) * sizeof(uint32_t) * 2 + 2);
    size_t cheatIndex = 0;
    for(size_t i = 0; tab[i] != NULL; i++) {
        //ignore titles and lines that could be too long for an actual cheat
        if (tab[i][0] == '#') {
            continue;
        }
        //gameshark codes are exactly this length
        if (strlen(tab[i]) < 12 || strlen(tab[i]) > 15) {
            continue;
        }
        char **splitCheat = ft_split(tab[i], ' ');
        long cheatValue1 = strtol(splitCheat[0], NULL, 16);
        long cheatValue2 = strtol(splitCheat[1], NULL, 16);
        cheats[cheatIndex] = cheatValue1;
        cheats[cheatIndex + 1] = cheatValue2;
        free_tab(splitCheat);
        cheatIndex += 2;
    }
    free_tab(tab);

    cheats[cheatIndex] = 0;
    cheats[cheatIndex + 1] = 0;
    menu->boot_params->cheat_list = cheats;
    return CHEAT_LOAD_OK;
}