/* Definitions for functions declared in ui_util.h */

#include <glib.h>
#include <glib/gstdio.h>	// g_mkdir & g_creat
#include <fcntl.h>	// mkdir & creat
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sendfile.h>

#include "libs/file_paths.h"
#include "libs/trie.h"
#include "libs/dict_handling/dict_handling.h"

#include "libs/file_checks.h"


/* ============================================================================================= */


int check_config_file(void) {

	gchar *config_dir = g_build_filename(g_get_user_config_dir(), GAME_CONFIG_DIR, (char *)NULL);
	gchar *config_file_path =  g_build_filename(g_get_user_config_dir(), GAME_CONFIG_DIR, GAME_CONFIG_NAME, (char *)NULL);


    if(!g_file_test(config_file_path, G_FILE_TEST_EXISTS)) {
		// Config file doesn't exist

		if(!g_file_test(config_dir, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR)) {	// check if the config dir already exists
			if(g_mkdir(config_dir, 0700) == -1) {		// Create subdirectory in $HOME/.config/ (+ test return value)
				g_free(config_dir);
				g_free(config_file_path);
				return 2;
			}
		}
		if(g_creat(config_file_path, 0700) == -1) {		// Create config file (+ test return value)
			g_free(config_dir);
			g_free(config_file_path);
			return 2;
		}

		GKeyFile *game_settings = g_key_file_new();
		GKeyFileFlags conf_flags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;
		GError *conf_error = NULL;

		g_key_file_load_from_file(game_settings, config_file_path, conf_flags, &conf_error);
		// Set the letters and rounds
		g_key_file_set_integer(game_settings, "Settings", "letters", 10);
		g_key_file_set_integer(game_settings, "Settings", "rounds", 10);
		g_key_file_save_to_file(game_settings, config_file_path, &conf_error);

		// Free all allocated memory
		g_key_file_free(game_settings);
		g_free(config_dir);
    	g_free(config_file_path);

      	return 17;

    }else {
		// Config file exists

		g_free(config_dir);
		g_free(config_file_path);

		return 0;
	}
}


/* --------------------------------------------------------------------------------------------- */


int check_dict_file(void) {

	gchar *data_dir = g_build_filename(g_get_user_data_dir(), GAME_DATA_DIR, (char *)NULL);
	gchar *dict_file_path =  g_build_filename(g_get_user_data_dir(), GAME_DATA_DIR, DICT_NAME, (char *)NULL);
	gchar *default_dict_file_path = g_build_filename(DATADIR, GAME_DATA_DIR, DEFAULT_DICT_NAME, (char *)NULL);


	if(!g_file_test(dict_file_path, G_FILE_TEST_EXISTS)) {
		// Trie json file doesn't exist

		if(!g_file_test(data_dir, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR)) {	// check if the data dir already exists
			if(g_mkdir(data_dir, 0700) == -1) {	// Create subdirectory in $HOME/.local/share/
				g_free(data_dir);
				g_free(dict_file_path);
				return 2;
			}
		}		
		if(g_creat(dict_file_path, 0700) == -1) { // Create dictionary file
			g_free(data_dir);
			g_free(dict_file_path);
			return 2;
		}

		// The dictionary file doesn't exist -> copy it from /usr/local/share/scrabble-1d/

		int dict_src, dict_dest;	// input and output file descriptors
		struct stat stat_buf;		// stats for source file
		off_t offset = 0;

		dict_src = open(default_dict_file_path, O_RDONLY);
		if(dict_src == -1) return 2;	// the program isn't installed correctly -> the default dictionary file is missing!
		fstat(dict_src, &stat_buf);		// save stats of source file

		dict_dest = open(dict_file_path, O_WRONLY);

		sendfile(dict_dest, dict_src, &offset, stat_buf.st_size);	// copy dict_src to dict_dest

		close(dict_dest);
		close(dict_src);

		g_free(data_dir);
		g_free(dict_file_path);

		return 17;

	}else {
		// Trie json file exists

		// Check if it has been modified while the game wasn't running
		

		g_free(data_dir);
		g_free(dict_file_path);

		return 0;
	}
}


/* --------------------------------------------------------------------------------------------- */


int check_trie_json_file(void) {

	gchar *data_dir = g_build_filename(g_get_user_data_dir(), GAME_DATA_DIR, (char *)NULL);
	gchar *dict_json_path =  g_build_filename(g_get_user_data_dir(), GAME_DATA_DIR, DICT_JSON_NAME, (char *)NULL);


	if(!g_file_test(dict_json_path, G_FILE_TEST_EXISTS)) {
		// Trie json file doesn't exist

		if(!g_file_test(data_dir, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR)) {	// check if the data dir already exists
			if(g_mkdir(data_dir, 0700) == -1) {	// Create subdirectory in $HOME/.local/share/
				g_free(data_dir);
				g_free(dict_json_path);
				return 2;
			}
		}
		if(g_creat(dict_json_path, 0700) == -1) {	// Create json file
			g_free(data_dir);
			g_free(dict_json_path);
			return 2;
		}

		// The trie json file doesn't exist -> the trie structure also doesn't exist -> generate it!

		struct node_t *trie_root = dictToTrie();   // generate trie structure from dictionary
		if(trie_root == NULL) {
			trie_delete(trie_root);
			g_free(data_dir);
			g_free(dict_json_path);
			g_free(trie_root);
			return 2;
		}

		gboolean dict_err = trieToJson(trie_root);   // write the generated trie to the json file
		if(dict_err != 0) {
			trie_delete(trie_root);
			g_free(data_dir);
			g_free(dict_json_path);
			g_free(trie_root);
			return 2;
		}

		// Free all allocated memory
		trie_delete(trie_root);
		g_free(trie_root);
		g_free(data_dir);
		g_free(dict_json_path);

		return 17;

	}else {
		// Trie json file exists

		g_free(data_dir);
		g_free(dict_json_path);

		return 0;
	}
}
