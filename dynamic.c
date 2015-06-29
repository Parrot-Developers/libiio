/*
 * libiio - Library for interfacing industrial I/O (IIO) devices
 *
 * Copyright (C) 2015 Parrot SA
 * Author: Romain Roffé <romain.roffe@parrot.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * */

#include "debug.h"
#include "iio-private.h"

#include <dlfcn.h>

#ifndef PLUGINS_DEFAULT_DIR
#define PLUGINS_DEFAULT_DIR "/usr/lib/libiio-plugins/libiio-"
#endif

struct iio_context * iio_create_context(const char *name)
{
	char path[1024];
	const char *xml;
	size_t xml_len;
	struct iio_context *ctx;
	struct iio_backend *backend;
	void *lib = NULL;

	if (!name)
		return NULL;

	/* Open plugin */
	snprintf(path, sizeof(path), PLUGINS_DEFAULT_DIR"%s.so", name);
	lib = dlopen(path, RTLD_NOW);
	if (!lib) {
		WARNING("Dynamic backend %s not found\n", name);
		return NULL;
	}

	backend = dlsym(lib, "backend");
	if (!backend) {
		WARNING("No symbol \"backend\" found in dynamic backend %s\n",
				name);
		goto close_lib;
	}

	/* Init the plugin */
	if (backend->init)
		backend->init();

	/* Create context from plugin's XML description */
	if (!backend->get_xml) {
		WARNING("No get_xml() method provided by dynamic backend %s\n",
				name);
		goto close_lib;
	}

	xml = backend->get_xml(&xml_len);
	if (!xml || xml_len == 0) {
		WARNING("Empty XML provided by dynamic backend %s\n", name);
		goto close_lib;
	}

	ctx = iio_create_xml_context_mem(xml, xml_len);
	if (!ctx) {
		WARNING("Invalid XML provided by dynamic backend %s\n", name);
		goto close_lib;
	}

	/* Override name and operations */
	iio_context_set_name(ctx, name);

	if (!backend->ops) {
		WARNING("No operation provided by dynamic backend %s\n", name);
		goto close_lib;
	}

	iio_context_set_ops(ctx, backend->ops);

	return ctx;

clean_ctx:
	free(ctx);
close_lib:
	dlclose(lib);

	return NULL;
}
