#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include "args.h"

static const char backlight_directory_path[] = "/sys/class/backlight";
static char file_path[256];
static char buffer[32];

static void act(action *action);
static void act_on(action *action, struct dirent *directory);
static int read_int(int fd);

int main(int argc, char **argv)
{
	action *action = parse_args(argc, argv);

	if (action == NULL)
	{
		printf("Help...\n");
	}
	else
	{
		act(action);
	}

	return 0;
}

void act(action *action)
{
	// open and verify baclight directory
	DIR *directory = opendir(backlight_directory_path);
	if (directory == NULL)
	{
		fprintf(stderr, "Failed to open %s", backlight_directory_path);
		return;
	}

	// iterate entries in backlight directory
	struct dirent *entry;
	while (entry = readdir(directory))
	{
		if (entry->d_name[0] == '.') continue;  // skip anything starting with '.'
		if (action->target != NULL && strcmp(entry->d_name, action->target) != 0) continue;  // skip anything that does not match our target, if we have a target

		act_on(action, entry);
	}

	// close backlight directory
	closedir(directory);
}

void act_on(action *action, struct dirent *directory)
{
	// read the maximum brightness
	sprintf(file_path, "%s/%s/%s", backlight_directory_path, directory->d_name, "max_brightness");
	int max_brightness_fd = open(file_path, O_RDONLY);
	if (max_brightness_fd == -1) return;
	int max_brightness = read_int(max_brightness_fd);
	close(max_brightness_fd);

	// open the brightness file
	sprintf(file_path, "%s/%s/%s", backlight_directory_path, directory->d_name, "brightness");
	int brightness_fd = open(file_path, O_RDWR);
	if (brightness_fd == -1) return;

	// figure out what the brightness setting will be
	int change = (float) max_brightness / 100 * action->change;
	int new_brightness;

	if (action->direction != 0)
	{
		int brightness = read_int(brightness_fd);

		if (action->direction == 1)
			new_brightness = brightness + change;
		else
			new_brightness = brightness - change;
	}
	else
		new_brightness = change;

	// TODO ensure 0 <= new_brightness <= max_brightness

	int bytes_written = sprintf(buffer, "%d\n", new_brightness);
	write(brightness_fd, buffer, bytes_written);

	// close the brightness file
	close(brightness_fd);
}

int read_int(int fd)
{
	int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	buffer[bytes_read] = '\0';
	return atoi(buffer);
}
