/*
 *	Functions that check for game files
 *
 *  Copyright (C) 2021 Yassen Efremov
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "libs/file_checks.h"

#include "libs/file_paths.h"
#include "libs/trie.h"
#include "libs/dict_handling/dict_handling.h"

#include <glib.h>
#include <glib/gstdio.h>	// g_mkdir & g_creat

#include <fcntl.h>	// mkdir & creat
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <errno.h>
#include <time.h>


/* ============================================================================================= */


int check_config_file(int *letters, int *rounds) {
	int errno_save = 0;

	gchar *config_dir = g_build_filename(g_get_user_config_dir(), GAME_DIR, (char *)NULL);
	gchar *config_file_path =  g_build_filename(g_get_user_config_dir(), GAME_DIR, GAME_CONFIG_NAME, (char *)NULL);

	GKeyFile *game_settings = g_key_file_new();
	GKeyFileFlags conf_flags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;
	GError *conf_error = NULL;


    if(!g_file_test(config_file_path, G_FILE_TEST_EXISTS)) {
		// Config file doesn't exist

		if(!g_file_test(config_dir, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR)) {	// check if the config dir already exists
			errno = 0;
			if(g_mkdir(config_dir, 0700) == -1) {		// Create subdirectory in $HOME/.config/ (+ test return value)
				errno_save = errno;
				goto CLEANUP;	// At the END of the function!
			}
		}
		errno = 0;
		if(g_creat(config_file_path, 0700) == -1) {		// Create config file (+ test return value)
			errno_save = errno;
			goto CLEANUP;	// At the END of the function!
		}

		g_key_file_load_from_file(game_settings, config_file_path, conf_flags, &conf_error);
		g_key_file_set_integer(game_settings, "Settings", "letters", 10);	// Set letters
		g_key_file_set_integer(game_settings, "Settings", "rounds", 10);	// Set rounds
		g_key_file_save_to_file(game_settings, config_file_path, &conf_error);

		g_key_file_free(game_settings);
    }

	// Update the settings in-game.

	g_key_file_load_from_file(game_settings, config_file_path, conf_flags, &conf_error);
	*letters = g_key_file_get_integer(game_settings, "Settings", "letters", &conf_error);	// Get letters
	*rounds = g_key_file_get_integer(game_settings, "Settings", "rounds", &conf_error);		// Get rounds

	g_key_file_free(game_settings);


	CLEANUP:	/* <========================== */
	g_free(config_dir);
	g_free(config_file_path);

	return errno_save;
}


/* --------------------------------------------------------------------------------------------- */


int check_dict_file(void) {
	int errno_save = 0;

	gchar *data_dir = g_build_filename(g_get_user_data_dir(), GAME_DIR, (char *)NULL);
	gchar *dict_file_path =  g_build_filename(g_get_user_data_dir(), GAME_DIR, DICT_NAME, (char *)NULL);
	gchar *dict_timestamp_path = g_build_filename(g_get_user_data_dir(), GAME_DIR, DICT_TIMESTAMP_NAME, (char *)NULL);
	// The DATADIR macro may appear undefined because it's handled by automake when compiling
	gchar *default_dict_file_path = g_build_filename(DATADIR, GAME_DIR, DEFAULT_DICT_NAME, (char *)NULL);

	struct stat dict_stat;		// stats for dictionary file


	if(!g_file_test(dict_file_path, G_FILE_TEST_EXISTS)) {
		// Dictionary file doesn't exist

		if(!g_file_test(data_dir, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR)) {	// check if the data dir already exists
			errno = 0;
			if(g_mkdir(data_dir, 0700) == -1) {	// Create subdirectory in $HOME/.local/share/
				errno_save = errno;
				goto CLEANUP;	// At the END of the function!
			}
		}
		errno = 0;		
		if(g_creat(dict_file_path, 0700) == -1) { // Create dictionary file
			errno_save = errno;
			goto CLEANUP;	// At the END of the function!
		}

		// The dictionary file doesn't exist -> copy it from /usr/local/share/scrabble-1d/default_dictionary.txt

		int src_dict, dest_dict;	// input and output file descriptors
		off_t offset = 0;
		int default_dict_err;
		GError *dict_err = NULL;

		src_dict = g_open(default_dict_file_path, O_RDONLY);
		fstat(src_dict, &dict_stat);		// save stats of source file
		dest_dict = g_open(dict_file_path, O_WRONLY);

		default_dict_err = sendfile(dest_dict, src_dict, &offset, dict_stat.st_size);	// Copy src_dict to dest_dict
		if(default_dict_err == -1) {
			// the program isn't installed correctly -> the default dictionary file is missing!
			errno_save = ENOENT;
			goto CLEANUP;	// At the END of the function!
		}

		g_close(dest_dict, &dict_err);
		g_close(src_dict, &dict_err);
	}


	if(!g_file_test(dict_timestamp_path, G_FILE_TEST_EXISTS)) {
		// Timestamp file doesn't exist

		errno = 0;
		if(g_creat(dict_timestamp_path, 0700) == -1) { // Create timestamp file
			errno_save = errno;
			goto CLEANUP;	// At the END of the function!
		}

		// Save the dictionary file modification time

		stat(dict_file_path, &dict_stat);	// save stats of dictionary file
		time_t curr_mtime = dict_stat.st_mtime;

		FILE *dict_timestamp = g_fopen(dict_timestamp_path, "w");
		g_fprintf(dict_timestamp, "%ld", curr_mtime);
		fclose(dict_timestamp);
	}


	CLEANUP:	/* <========================== */
	g_free(data_dir);
	g_free(dict_file_path);
	g_free(dict_timestamp_path);
	g_free(default_dict_file_path);

	return errno_save;
}


/* --------------------------------------------------------------------------------------------- */


int check_trie_json_file(void) {
	int errno_save = 0;

	gchar *data_dir = g_build_filename(g_get_user_data_dir(), GAME_DIR, (char *)NULL);
	gchar *dict_json_path =  g_build_filename(g_get_user_data_dir(), GAME_DIR, DICT_JSON_NAME, (char *)NULL);
	

	if(!g_file_test(dict_json_path, G_FILE_TEST_EXISTS)) {
		// Trie json file doesn't exist

		if(!g_file_test(data_dir, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR)) {	// check if the data dir already exists
			errno = 0;
			if(g_mkdir(data_dir, 0700) == -1) {	// Create subdirectory in $HOME/.local/share/
				errno_save = errno;
				goto CLEANUP;	// At the END of the function!
			}
		}
		errno = 0;
		if(g_creat(dict_json_path, 0700) == -1) {	// Create json file
			errno_save = errno;
			goto CLEANUP;	// At the END of the function!
		}

		// The trie json file doesn't exist -> the trie structure also doesn't exist -> generate it!

		struct node_t *trie_root = dictToTrie();   // generate trie structure from dictionary
		if(trie_root == NULL) {
			trie_delete(trie_root);
			g_free(trie_root);
			errno_save = ECANCELED;
			goto CLEANUP;	// At the END of the function!
		}

		gboolean dict_err = trieToJson(trie_root);   // write the generated trie to the json file
		if(dict_err != 0) {
			trie_delete(trie_root);
			g_free(trie_root);
			errno_save = ECANCELED;
			goto CLEANUP;	// At the END of the function!
		}

		trie_delete(trie_root);
		g_free(trie_root);

	}else {
		// Trie json file exists

		// Check if the dictionary has been modified while the game hasn't been running

		gchar *dict_file_path = g_build_filename(g_get_user_data_dir(), GAME_DIR, DICT_NAME, (char *)NULL);
		gchar *dict_timestamp_path = g_build_filename(g_get_user_data_dir(), GAME_DIR, DICT_TIMESTAMP_NAME, (char *)NULL);

		// Get the currnet modification time of the dictionary file
		struct stat dict_stat;		// stats for dictionary file
		stat(dict_file_path, &dict_stat);	// save stats of dictionary file
		time_t curr_mtime = dict_stat.st_mtime;
		g_free(dict_file_path);

		// Get the old modification time from the timestamp file
		FILE *dict_timestamp = g_fopen(dict_timestamp_path, "r");
		gchar old_mtime_str[sizeof(time_t)];
		fread(old_mtime_str, sizeof(time_t), 1, dict_timestamp);	// read the timestamp as a string
		fclose(dict_timestamp);
		g_free(dict_timestamp_path);

		time_t old_mtime = atoi(old_mtime_str);		// convert modification time to a number

		// Compare current and saved time 
		if(curr_mtime > old_mtime) {
			// Regenerate the trie json file

			struct node_t *trie_root = dictToTrie();   // generate trie structure from dictionary
			if(trie_root == NULL) {
				trie_delete(trie_root);
				g_free(trie_root);
				errno_save = ECANCELED;
				goto CLEANUP;	// At the END of the function!
			}

			gboolean dict_err = trieToJson(trie_root);   // write the generated trie to the json file
			if(dict_err != 0) {
				trie_delete(trie_root);
				g_free(trie_root);
				errno_save = ECANCELED;
				goto CLEANUP;	// At the END of the function!
			}

			trie_delete(trie_root);
			g_free(trie_root);
		}
	}


	CLEANUP:	/* <========================== */
	g_free(data_dir);
	g_free(dict_json_path);

	return errno_save;
}
