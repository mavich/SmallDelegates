# SmallDelegates

Small, headers only, delegates library for C++.

It faster and easier understanding ans to use than the standart STL implementation.

## Usage
To use it, just simply header files wherever you want it, and include it:

`#include<{HEADERS_DIR}/SmallDelegates/main.h>`

Or you can include individual parts of it:

`#include<{HEADERS_DIR}/SmallDelegates/{HEADER_NAME}.h>`

Passable **HEADER_NAME**s

* **_functions_** - includes delegats for static functions or lambda functions.
* **_methods_** - includes delegats for working with member functions.
* **_events_** - includes a container for other delegats and can execute them.
* **_base_** - includes base interface for all delegats.

All library hidden in the namespace **delegates_ns** for denying name collisions.

## Classes

### IDelegate< class R ( class ...Args ) >

Base interface for all delegates.

* **R** - Function return type.
* **...Args** - Function arguments type list.

####Member functions
* **getHashKey** - Abstruct funtion for getting hash key. Used in friend overloaded compare function.
 * **NOTE**: The result of this function is not fit to be serialized. It uses too much parameters dependent on the build, session and a computer.
 * **signature**: `virtual size_t getHashKey( void ) const = 0;`
 * **arguments**: *none*.
 * **return**: hash key.
* **copy** - Abstruct function. Used to make copy of objects of classes inheritors.
 * **signature**: `virtual IDelegate&   copy( void ) const = 0;`
 * **arguments**: *none*.
 * **return**: copy of an object.
* **operator( )** - Abstruct function used to invock.
 * **signature**: `virtual R operator( ) ( Args... args ) const = 0;`
 * **arguments**: depends on the value of *Args...* list.
 * **return**: value of type *R*.

####Friend functions
* **operator==** - Compare objects using they hash keys.
 * **signature**: `friend bool			operator==( const IDelegate& left, const IDelegate& right );`
 * **return**: comparison result of hash keys.

### FunctionD< class R ( class ...Args ) >

Class for working with static functions and lambda functions (for now it working only with lambdas without captured values).

* **R** - Function return type.
* **...Args** - Function arguments type list.
* **FType** - Function type which delegat can hold.
`typedef R ( *FType )( Args... );`

####Member functions
* **(constructor)**
 * **signature**:
 * `FunctionD( FType func )` - create delegat using pointer to function.
 * `FunctionD( const FunctionD& other )` - create a copy of other delegat.
* **operator=** - assignment operator.
 * **signature**:
 * `FunctionD& operator= ( const FunctionD& functionD );`
 * `FunctionD& operator= ( const FType function );`
 * **return**: reference to this object.

####Friend functions
* **operator==**
 * **signature**:
 * `friend bool operator== ( const FunctionD& left, const FType right );`
 * `friend bool operator== ( const FType left, const FunctionD& right );`
 * **return**: comparison result.

####Fields
* `FType function` - pointer to function.

### MethodD< class T, class R ( class ...Args ) >

Class for working with member functions(methods).

* **T** - Object type.
* **R** - Method return type.
* **...Args** - Method arguments type list.
* **FType** - Method type which delegat can hold.
`typedef R ( T::*FType )( Args... );`

####Member functions
* **(constructor)**
 * **signature**:
 * `MethodD( T* obj, FType func );` - create delegat using pointer to object and pointer to method.
 * `MethodD( const MethodD& other );` - create a copy of other delegat.
* **operator=** - assignment operator.
 * **signature**:
 * `MethodD& operator= ( const MethodD& functionD );`
 * `MethodD& operator= ( const FType function );`
 * `MethodD& operator= ( T* object )`
 * **return**: reference to this object.

####Friend functions
* **operator==**
 * **signature**:
 * `friend bool operator== ( const MethodD& left, const T* right );`
 * `friend bool	operator== ( const T* left, const MethodD& right );`
 * `friend bool operator== ( const MethodD& left, const FType right );`
 * `friend bool operator== ( const FType left, const MethodD& right );`
 * **return**: comparison result.

####Fields
* `T* object` - pointer to the object.
* `FType function` - pointer to function.

### Event< class ...Args >

Class for working with collection of delegates inherited from **IDelegates< void ( Args... ) >**.
Because of inheritance it can hold another events to.

> **NOTE**: It use something similar to primitive linked list, so it may be pretty slow.

* **...Args** - Method arguments type list.

####Member functions
* **(constructor)**
 * **signature**:
 * `Event( void );` - create empty object.
 * `Event( const Event& other );` - create a copy of other delegat.
 * `Event( Event&& other )` - move constructor.
* **operator=** - assignment operator.
 * **signature**:
 * `Event& operator= ( const Event& other );`
 * `Event& operator= ( Event&& other );`
 * **return**: reference to this object.
* **operator+=** - signs the delegate to the event.
 * **signature**:
 * `Event& operator+= ( const Delegate& delegate )`
 * **return**: reference to this object.
 * **analogous**:
 * `Event& add( const Delegate& delegate )`
* **operator-=** - unsigns the delegate from the event.
 * **signature**:
 * `Event& operator-= ( const Delegate& delegate )`
 * **return**: reference to this object.
 * **analogous**:
 * `Event& remove( const Delegate& delegate )`

## Performance

For performance testing be used **for** cycle and **EMPTY** function (time in ms).

**CPU**: Intel(R) Pentium(R) CPU  N3530 2.16GHz 4 cores.

**System**: Windows 8.1 Pro x64.

### FunctionD vs std::function

#### MinGW x64

|       |STL  |      |SmDs |      |
|-------|-----|------|-----|------|
|Num    |Debug|Relese|Debug|Relese|
|1kk    |27   |2     |7    |0     |
|2.5kk  |67   |7     |17   |0     |
|5kk    |137  |14    |35   |0     |
|10kk   |274  |34    |69   |0     |

#### VSC x64

|       |STL  |      |SmDs |      |
|-------|-----|------|-----|------|
|Num    |Debug|Relese|Debug|Relese|
|1kk    |72   |4     |24   |0     |
|2.5kk  |184  |8     |64   |4     |
|5kk    |376  |16    |132  |12    |
|10kk   |756  |36    |268  |24    |

### MethodD vs std::function

#### MinGW x64

|       |STL  |      |SmDs |      |
|-------|-----|------|-----|------|
|Num    |Debug|Relese|Debug|Relese|
|1kk    |66   |3     |12   |0     |
|2.5kk  |166  |9     |31   |0     |
|5kk    |390  |18    |61   |0     |
|10kk   |738  |38    |121  |0     |

#### VSC x64

>In the issue of an error in VCS STL realization of **_std::function_** class, there is no comparison.

|       |SmDs |      |
|-------|-----|------|
|Num    |Debug|Relese|
|1kk    |28   |0     |
|2.5kk  |62   |8     |
|5kk    |140  |16    |
|10kk   |286  |32    |

### Event

Used 3 delegates to **EMPTY** static functions.

#### MinGW x64

|       |SmDs |      |
|-------|-----|------|
|Num    |Debug|Relese|
|1kk    |50   |25    |
|2.5kk  |125  |62    |
|5kk    |272  |122   |
|10kk   |547  |246   |

#### VSC x64

|       |SmDs |      |
|-------|-----|------|
|Num    |Debug|Relese|
|1kk    |140  |8     |
|2.5kk  |351  |20    |
|5kk    |712  |40    |
|10kk   |1442 |84    |