#pragma once

#include <string>

namespace glimac {

class FilePath {
public:
#ifdef _WIN32
    static const char PATH_SEPARATOR = '\\';
#else
    static const char PATH_SEPARATOR = '/';
#endif

    FilePath() = default;

    FilePath(const char* filepath): m_FilePath(filepath) {
        format();
    }

    FilePath(const std::string& filepath): m_FilePath(filepath) {
        format();
    }

    operator std::string() const { return m_FilePath; }

    const std::string& str() const { return m_FilePath; }

    const char* c_str() const { return m_FilePath.c_str(); }

    bool empty() const {
        return m_FilePath.empty();
    }

    /*! returns the path of a filepath */
    FilePath dirPath() const {
        size_t pos = m_FilePath.find_last_of(PATH_SEPARATOR);
        if (pos == std::string::npos) { return FilePath(); }
            return m_FilePath.substr(0, pos);
        }

    /*! returns the file of a filepath  */
    std::string file() const {
        size_t pos = m_FilePath.find_last_of(PATH_SEPARATOR);
        if (pos == std::string::npos) { return m_FilePath; }
        return m_FilePath.substr(pos + 1);
    }

    /*! returns the file extension */
    std::string ext() const {
        size_t pos = m_FilePath.find_last_of('.');
        if (pos == std::string::npos || pos == 0) { return ""; }
        return m_FilePath.substr(pos + 1);
    }

    bool hasExt(const std::string& ext) const {
        int offset = (int) m_FilePath.size() - (int) ext.size();
        return offset >= 0 && m_FilePath.substr(offset, ext.size()) == ext;
    }

    /*! adds file extension */
    FilePath addExt(const std::string& ext = "") const {
        return FilePath(m_FilePath + ext);
    }

    /*! concatenates two filepaths to this/other */
    FilePath operator +(const FilePath& other) const {
        if (m_FilePath.empty()) {
            return other;
        } else {
            if(other.empty()) {
                return m_FilePath;
            }
            FilePath copy(*this);
            if(other.m_FilePath.front() != PATH_SEPARATOR) {
                copy.m_FilePath += PATH_SEPARATOR;
            }
            copy.m_FilePath += other.m_FilePath;
            return copy;
        }
    }

    bool operator ==(const FilePath& other) const {
        return other.m_FilePath == m_FilePath;
    }

    bool operator !=(const FilePath& other) const {
        return !operator ==(other);
    }

    /*! output operator */
    friend std::ostream& operator<<(std::ostream& cout, const FilePath& filepath) {
        return (cout << filepath.m_FilePath);
    }

private:
    void format() {
        for (size_t i = 0; i < m_FilePath.size(); ++i) {
            if (m_FilePath[i] == '\\' || m_FilePath[i] == '/') {
                m_FilePath[i] = PATH_SEPARATOR;
            }
        }
        while (!m_FilePath.empty() && m_FilePath.back() == PATH_SEPARATOR) {
            m_FilePath.pop_back();
        }
    }

    std::string m_FilePath;
};

}

namespace std {
  template <>
  struct hash<glimac::FilePath> {
    std::size_t operator()(const glimac::FilePath& k) const {
        return std::hash<std::string>()(k.str());
    }
  };
}
