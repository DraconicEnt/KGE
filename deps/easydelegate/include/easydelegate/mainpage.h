/**
 *  @file mainpage.h
 *  @date 11/17/2016
 *  @version 3.0
 *  @brief Main page file for use with Doxygen. This file does absolutely nothing otherwise.
 *  @author <a href="http://dx.no-ip.org">Robert MacGregor</a>
 *  @example example.cpp
 *
 *  @mainpage Welcome
 *  EasyDelegate is, as the name implies, a <a href="http://msdn.microsoft.com/en-us/library/ms173171.aspx">delegate system.</a>
 *  It is written for C++11 with the intention to allow C++ programmers to easily create references to static and member methods
 *  that may be passed around like any other variable and invoked at any point in the code. This is helpful for a variety of situations,
 *  such as an <a href="http://en.wikipedia.org/wiki/Sink_(computing)">event/listener implementation</a>.
 *
 *  @section Example
 *  This quick example below demonstrates how simple it is to utilize the EasyDelegate library, though for a more complete example,
 *  please refer to example.cpp.
 *
 *  @code
 *  class MyCustomClass
 *  {
 *      public:
 *          unsigned int myMemberMethod(char *str, float flt, double dbl)
 *          {
 *              std::cout << "MyCustomClass::myMemberMethod: " << str << "," << flt << "," << dbl << std::endl;
 *              return 2;
 *          }
 *  };
 *
 *  int main(int argc, char *argv[])
 *  {
 *      // Typedef our event type for later shortcutting
 *      typedef EasyDelegate::DelegateSet<unsigned int, char*, float, double> MyEventType;
 *
 *      // Instantiate our delegate set and the custom class type.
 *      MyEventType myDelegateSet;
 *      MyCustomClass *myCustomClassInstance = new MyCustomClass();
 *
 *      // Instantiate our member Delegate; we obviously want it to persist even when out of scope in a production environment so we use a pointer.
 *      // Notice how the class type is only cared about here rather than in the DelegateSet typedef or in the push_back call below. This is because both
 *      // StaticDelegate and MemberDelegate types may be stored in the same DelegateSet as they have the same signature, the only difference is that
 *      // MemberDelegate types need a this pointer to call against.
 *      MyEventType::MemberDelegateType<MyCustomClass> *myMemberDelegate = new MyEventType::MemberDelegateType<MyCustomClass>(myCustomClassInstance, &MyCustomClass::myMemberMethod);
 *
 *      // Register the the myMemberMethod of MyCustomClass
 *      myDelegateSet.push_back(myMemberDelegate);
 *
 *      // You can invoke the DelegateSet in the same way you invoke the delegate directly
 *      myDelegateSet.invoke("Foo", 3.14, 3.14159);
 *      myMemberDelegate->invoke("Foo", 3.14, 3.14159);
 *
 *      delete myMemberDelegate;
 *      delete myCustomClassInstance; // Note that this causes any related MemberDelegate instances to become 'bad'.
 *  }
 *  @endcode
 *
 *  The above code sample will output:
 *
 *  "MyCustomClass::myMemberMethod: Foo,3.14,3.14159"
 *
 *  "MyCustomClass::myMemberMethod: Foo,3.14,3.14159"
 *
 *  @section deferred Deferred Callers
 *  EasyDelegate also supports deferred call objects in C++ using the same implementations
 *  that the regular EasyDelegate library uses. They provide you with the ability to cache
 *  a call and later dispatch it completely anonymously if so desired. Take the following code sample:
 *
 *  @code
 *  typedef EasyDelegate::DeferredStaticCaller<void, const float&> MyDeferredCaller;
 *
 *  static void someMethod(const float& in)
 *  {
 *      std::cout << "Got float: " << in << std::endl;
 *  }
 *
 *  int main(int argc, char *argv[])
 *  {
 *      EasyDelegate::MyDeferredCaller* caller = new MyDeferredCaller(someMethod, 3.14f);
 *      EasyDelegate::IDeferredCaler* generic = reinterpret_cast<EasyDelegate::IDeferredCaller>(caller);
 *      generic->genericDispatch();
 *      delete generic;
 *  }
 *  @endcode
 *
 *  The output of this code will be "Got float: 3.14".
 *
 *  @section Support Supported Compilers and Operating Systems
 *  EasyDelegate has been compiled and known to run on the following systems:
 *  <ul>
 *      <li><a href="https://gcc.gnu.org/">GCC 4.8.4</a> on Gentoo AMD64</li>
 *      <li><a href="http://www.mingw.org/">MinGW 4.8.1</a> on Windows 7 x64</li>
 *      <li><a href="http://www.microsoft.com/en-us/download/details.aspx?id=34673">Microsoft Visual Studio C++ 2012 Express</a> on Windows 7 x64</li>
 *  </ul>
 *
 *  It should compile and run underneath of any compiler that at least supports <a href="http://en.wikipedia.org/wiki/Variadic_template">variadic templates</a>,
 *  <a href="http://www.cplusplus.com/reference/tuple/">std::tuple</a> (used by CachedDelegate types), <a href="http://www.cplusplus.com/reference/set/set/">std::set</a>,
 *  <a href="http://www.cplusplus.com/reference/unordered_set/unordered_set/">std::unordered_set</a> but preferably has full C++11 support, though. If the use of the STL
 *  is not an option, then it should be relatively easy to switch EasyDelegate to the preferred container types for your project.
 *
 *  <b>Note for MinGW/GCC Users:</b><br>
 *  EasyDelegate has been known to fail compilation (and possibly crash the compiler) on MinGW 4.7, and the same bugs probably exist in *nix native GCC. Please ensure
 *  that you are running at least GCC 4.8 when building a project with EasyDelegate.
 *
 *  @section Limits Limitations
 *  Due to the nature of the library, there is one limitation which derives from the usage of templates which is the inability to work with <a href="http://en.cppreference.com/w/cpp/utility/variadic">variadic methods</a>
 *  because the ellipses ('...') involved are interpreted as a variadic template argument expansion and thusly will fail to compile. A work around for this if such
 *  functionality is necessary to your project would be to use a void* declaration instead and use that to pass a struct in which is then casted to the proper struct
 *  type on the called method's end. There may be a way to properly implement variadic method support, but I have not come up with anything yet.
 *
 *  @section Links Links & Attributions
 *  The vast majority of this library was written by Robert MacGregor, licensed under the MIT license for the open source community. There exists
 *  two small types that were pulled from StackOverflow which were quite imperative to the functioning of the deferred calling systems:
 *  <ul>
 *      <li><a href="http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer">The seq and gens types</a></li>
 *      <li><a href="http://stackoverflow.com/questions/12742877/remove-reference-from-stdtuple-members">The helpful no-reference tuple declaration</a></li>
 *  </ul>
 *
 *  Other Links:
 *  <ul>
 *      <li><a href="http://dx.no-ip.org">Author: Robert MacGregor</a></li>
 *      <li><a href="https://github.com/Ragora/EasyDelegate">EasyDelegate on GitHub</a></li>
 *  </ul>
 *
 *  @htmlonly
 *  <center><a href="https://www.biicode.com/Ragora/EasyDelegate"><img src="https://webapi.biicode.com/v1/badges/Ragora/Ragora/EasyDelegate/master"></a></center>
 *  @endhtmlonly
 */
