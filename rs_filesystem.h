#ifndef RS_FILESYSTEM_H
#define RS_FILESYSTEM_H

#if defined(__GNUC__) && (__GNUC__ == 8) && (__GNUC_MINOR__ == 1)

#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <string>
#include <system_error>

namespace fs {
class path {
public:
	path() = default;
	path(const std::string& p) : _path(p) {}
	std::string string() const { return _path; }
	operator std::string() const { return _path; }
private:
	std::string _path;
};

inline path u8path(const std::string& p) {
	return path(p);
}

inline bool create_directories(const std::string& p, std::error_code& ec) {
	QDir dir;
	bool ok = dir.mkpath(QString::fromStdString(p));
	if (!ok) ec = std::make_error_code(std::errc::io_error);
	return ok;
}
inline bool create_directories(const path& p, std::error_code& ec) {
	return create_directories(p.string(), ec);
}

inline uint64_t file_size(const path& p, std::error_code& ec) {
	QFileInfo info(QString::fromStdString(p.string()));
	if (!info.exists()) {
		ec = std::make_error_code(std::errc::no_such_file_or_directory);
		return 0;
	}
	return info.size();
}

inline bool remove(const path& p, std::error_code& ec) {
	QFile file(QString::fromStdString(p.string()));
	if (!file.exists()) return false;
	bool ok = file.remove();
	if (!ok) ec = std::make_error_code(std::errc::permission_denied);
	return ok;
}
}

#else

#include <filesystem>
namespace fs = std::filesystem;

#endif

#endif // RS_FILESYSTEM_H
