#ifndef RETURN_CODE_H
#define RETURN_CODE_H

#define NO_ERROR 0

#define UTILS_BITMAP_POSITION_OUT_OF_BOUNDS_ERROR 1

#define RM_ERROR 101
#define RM_RID_NOT_INITIALIZED_ERROR 102
#define RM_RECORD_NOT_INITIALIZED_ERROR 104
#define RM_RECORD_INVALID_LENGTH_ERROR 105
#define RM_FILEHANDLE_RECORD_NOT_FOUND_ERROR 107
#define RM_FILEHANDLE_NOT_INITIALIZED_ERROR 108
#define RM_FILEHANDLE_NO_EMPTY_POSITION_ERROR 109
#define RM_MANAGER_DUPLICATE_FILE_NAME_ERROR 110
#define RM_MANAGER_FILE_NOT_FOUND_ERROR 111
#define RM_MANAGER_FILE_CANNOT_OPEN_ERROR 112
#define RM_FILESCAN_INVALID_ATTR_LENGTH_ERROR 113

#endif  // RETURN_CODE_H