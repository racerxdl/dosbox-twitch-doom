/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *
 *  Copyright (C) 2019-2020  The dosbox-staging team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#if defined(WIN32)

#include "fs_utils.h"

#include <direct.h>
#include <io.h>

#include "compiler.h"

bool path_exists(const char *path) noexcept
{
	return (_access(path, 0) == 0);
}

std::string to_native_path(const std::string &path) noexcept
{
	if (path_exists(path))
		return path;
	return "";
}

int create_dir(const char *path, MAYBE_UNUSED uint32_t mode, uint32_t flags) noexcept
{
	const int err = _mkdir(path);
	if ((errno == EEXIST) && (flags & OK_IF_EXISTS)) {
		struct _stat pstat;
		if ((_stat(path, &pstat) == 0) &&
		    ((pstat.st_mode & S_IFMT) == S_IFDIR))
			return 0;
	}
	return err;
}

#endif
