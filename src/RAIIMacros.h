#pragma once

// Default the constructor
#define DEFAULT_CONSTRUCTOR(ClassName) \
    ClassName() = default;

// Default the destructor
#define DEFAULT_DESTRUCTOR(ClassName) \
    ~ClassName() = default;

// Default the copy constructor
#define DEFAULT_COPY_CONSTRUCTOR(ClassName) \
    ClassName(const ClassName&) = default;

// Default the copy assignment operator
#define DEFAULT_COPY_ASSIGNMENT(ClassName) \
    ClassName& operator=(const ClassName&) = default;

// Default the move constructor
#define DEFAULT_MOVE_CONSTRUCTOR(ClassName) \
    ClassName(ClassName&&) = default;

// Default the move assignment operator
#define DEFAULT_MOVE_ASSIGNMENT(ClassName) \
    ClassName& operator=(ClassName&&) = default;

// Delete the copy constructor
#define DELETE_COPY_CONSTRUCTOR(ClassName) \
    ClassName(const ClassName&) = delete;

// Delete the copy assignment operator
#define DELETE_COPY_ASSIGNMENT(ClassName) \
    ClassName& operator=(const ClassName&) = delete;

// Delete the move constructor
#define DELETE_MOVE_CONSTRUCTOR(ClassName) \
    ClassName(ClassName&&) = delete;

// Delete the move assignment operator
#define DELETE_MOVE_ASSIGNMENT(ClassName) \
    ClassName& operator=(ClassName&&) = delete;

// Fully default all RAII functions
#define DEFAULT_ALL_RAII_FUNCTIONS(ClassName) \
    DEFAULT_CONSTRUCTOR(ClassName)            \
    DEFAULT_DESTRUCTOR(ClassName)             \
    DEFAULT_COPY_CONSTRUCTOR(ClassName)       \
    DEFAULT_COPY_ASSIGNMENT(ClassName)        \
    DEFAULT_MOVE_CONSTRUCTOR(ClassName)       \
    DEFAULT_MOVE_ASSIGNMENT(ClassName)

// Delete all copy/move constructors and assignments
#define DELETE_ALL_COPY_MOVE_FUNCTIONS(ClassName) \
    DELETE_COPY_CONSTRUCTOR(ClassName)            \
    DELETE_COPY_ASSIGNMENT(ClassName)             \
    DELETE_MOVE_CONSTRUCTOR(ClassName)            \
    DELETE_MOVE_ASSIGNMENT(ClassName)

// Allow move but delete copy operations
#define NO_COPY_ALLOW_MOVE(ClassName) \
    DELETE_COPY_CONSTRUCTOR(ClassName) \
    DELETE_COPY_ASSIGNMENT(ClassName)  \
    DEFAULT_MOVE_CONSTRUCTOR(ClassName) \
    DEFAULT_MOVE_ASSIGNMENT(ClassName)

// Allow copy but delete move operations
#define NO_MOVE_ALLOW_COPY(ClassName) \
    DEFAULT_COPY_CONSTRUCTOR(ClassName) \
    DEFAULT_COPY_ASSIGNMENT(ClassName)  \
    DELETE_MOVE_CONSTRUCTOR(ClassName)  \
    DELETE_MOVE_ASSIGNMENT(ClassName)
