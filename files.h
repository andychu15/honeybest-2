/*
 * Security Hash Locking Module
 *
 * Copyright 2020 Moxa Inc.
 *
 * Author: Jimmy Chen <jimmy.chen@moxa.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

typedef struct hb_file_ll_t {
	unsigned int fid;	// security hook function open file by program
	uid_t uid;
	char *pathname;	// open file path
	struct list_head list;
} hb_file_ll;

hb_file_ll *search_file_record(unsigned int fid, uid_t uid, char *pathname);
int add_file_record(unsigned int fid, uid_t uid, char *pathname, int interact);

int read_file_record(struct seq_file *m, void *v);
ssize_t write_file_record(struct file *file, const char __user *buffer, size_t count, loff_t *pos);
int allow_file_whitelist(char *path);
