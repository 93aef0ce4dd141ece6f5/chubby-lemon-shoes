/*
 * Title: exec.c
 * Description: functions which concern executing
 *              or authentication go here
 *
 *
 *
 *
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * function to check if user
 * has authenticated with correct
 * password
 * return TRUE if correct else
 * return FALSE
 */
int check_pass (pMessage m) {
	if (strcmp (m->param, BOT_PWORD) == 0) {
		return TRUE;
	}

	return FALSE;
}

/*
 * function to add nickname to 
 * admin list
 * requires a bit of error checking
 * to see if larger than max admin
 * size (65535) and if admin array needs to
 * be expanded
 */
int add_admin (pAccount a, pMessage m) {
	// check if admins are maxed out
	if (a->num_admins < sizeof (unsigned short)) {
		// save previous admin size for iteration
		unsigned short prev_admin_size = a->num_admins;
		// check if admin array needs to be expanded
		if (a->num_admins <= sizeof (a->admins)/sizeof (*a->admins)) {
			// expand array with realloc
			a->admins = relloc (a->admins, a->num_admins*2);
			if (a->admins == NULL) {
				return FALSE;
			}
			// update admin size
			a->num_admins *= 2;

			// iterate through admins to NULL any unused array pointers
			int i;
			for (i = 0; i < prev_admin_size; i++);
			// initialise unused array pointers to NULL
			for (i; i < a->num_admins; i++) {
				a->admins[i] = NULL;
			}
		}
		// add new admin to next avaiable array element
		a->admins[prev_admin_size] = m->n_name;
	}

	return TRUE;
}

/*
 * function to check if user's
 * nickname is registered as an
 * admin
 * iterate through admin list with
 * strcmp and return non-zero if
 * is admin, else return 0
 */
int is_admin (pAccount a, pMessage m) {
	int i;
	for (i = 0; i < a->num_admins; i++) {
		if (strcmp (m->n_name, a->admins[i])) {
			return TRUE;
		}
	}

	return FALSE;
}