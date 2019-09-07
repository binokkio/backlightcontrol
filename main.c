#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include "args.h"
#include "int_file.h"

static const char backlight_directory_path[] = "/sys/class/backlight";
static char file_path[256];

static void act(action *action);
static void act_on(action *action, struct dirent *directory);

int main(int argc, char **argv)
{
	action *action = parse_args(argc, argv);

	if (action == NULL)
		printf("Usage: backlightcontrol set|increment|decrement amount [target]\n");
	else
		act(action);

	return 0;
}

void act(action *action)
{
	// open and verify backight directory
	DIR *directory = opendir(backlight_directory_path);
	if (directory == NULL)
	{
		fprintf(stderr, "Failed to open %s\n", backlight_directory_path);
		return;
	}

	// iterate entries in backlight directory
	int acts = 0;
	struct dirent *entry;
	while (entry = readdir(directory))
	{
		if (entry->d_name[0] == '.') continue;  // skip anything starting with '.'
		if (action->target != NULL && strcmp(entry->d_name, action->target) != 0) continue;  // skip anything that does not match our target, if we have a target

		act_on(action, entry);
		acts++;
	}

	if (!acts)
		if (action->target != NULL)
			fprintf(stderr, "Nothing in %s matched target \"%s\"\n", backlight_directory_path, action->target);
		else
			fprintf(stderr, "Nothing in %s\n", backlight_directory_path, action->target);

	// close backlight directory
	closedir(directory);
}

void act_on(action *action, struct dirent *directory)
{
	// read the maximum brightness
	sprintf(file_path, "%s/%s/%s", backlight_directory_path, directory->d_name, "max_brightness");
	int max_brightness_fd = open(file_path, O_RDONLY);
	if (max_brightness_fd == -1)
	{
		fprintf(stderr, "Failed to read max brightness from %s\n", file_path);
		return;
	}
	int max_brightness = read_int_from_file(max_brightness_fd);
	close(max_brightness_fd);

	// open the brightness file
	sprintf(file_path, "%s/%s/%s", backlight_directory_path, directory->d_name, "brightness");
	int brightness_fd = open(file_path, O_RDWR);
	if (brightness_fd == -1)
	{
		fprintf(stderr, "Failed to open %s\n", file_path);
		return;
	}

	// figure out what the brightness setting will be
	int change = (float) max_brightness / 100 * action->change;
	int new_brightness;

	if (action->direction != 0)
	{
		int brightness = read_int_from_file(brightness_fd);
		new_brightness = brightness + action->direction * change;
	}
	else
		new_brightness = change;

	// ensure 0 <= new_brightness <= max_brightness
	if (new_brightness < 0)
		new_brightness = 0;
	else if (new_brightness > max_brightness)
		new_brightness = max_brightness;

	// write the new brightness to the brightness file and close it
	write_int_to_file(brightness_fd, new_brightness);
	close(brightness_fd);
}
