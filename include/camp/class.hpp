// <insert copyright>

#ifndef CAMP_CLASS_HPP
#define CAMP_CLASS_HPP


#include <camp/config.hpp>
#include <camp/args.hpp>
#include <camp/error.hpp>
#include <camp/detail/classmanager.hpp>
#include <camp/detail/typeid.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>


namespace camp
{
template <typename T> class ClassBuilder;
class UserObject;
class Property;
class Function;
class Constructor;
class Value;
class ClassVisitor;

/**
 * \brief camp::Class represents a metaclass composed of properties and functions
 *
 * camp::Class is the main class of the CAMP API. It defines a metaclass, which
 * is an abstract representation of a C++ class with its own properties,
 * functions, constructors, base classes, etc.
 *
 * Classes are declared, bound to a C++ type and filled with the \c declare
 * template function.
 *
 * \code
 * class MyClass
 * {
 * public:
 *
 *     MyClass();
 *     int getProp() const;
 *     void setProp(int);
 *     std::string func();
 * };
 *
 * camp::Class::declare<MyClass>("MyClass")
 *     .constructor()
 *     .property("prop", &MyClass::getProp, &MyClass::setProp)
 *     .function("func", &MyClass::func);
 * \endcode
 *
 * It then provides a set of accessors to retrieve its member functions and properties.
 *
 * \code
 * const camp::Class& metaclass = camp::classByType<MyClass>();
 *
 * const Property& prop = metaclass.property("prop");
 * const Function& func = metaclass.function("func");
 * \endcode
 *
 * Another way to inspect a class, which is more type-safe, is to use a ClassVisitor.
 *
 * \code
 * MyVisitor visitor;
 * metaclass.visit(visitor);
 * \endcode
 *
 * It also allows to create and destroy instances of the bound C++ class.
 *
 * \code
 * MyClass* obj = metaclass.construct<MyClass>();
 * metaclass.destroy(obj);
 * \endcode
 *
 * \remark All function and property names are unique within the metaclass.
 *
 * \sa Class, EnumBuilder, Function, Property
 */
class Class : boost::noncopyable
{
public:

    /**
     * \brief Declare a new metaclass
     *
     * This is the function to call to create a new metaclass. The template
     * parameter T is the C++ class that will be bound to the metaclass.
     *
     * \param name Name of the metaclass in CAMP. This name identifies
     *             the metaclass and thus has to be unique
     *
     * \return A ClassBuilder<T> object that will provide functions
     *         to fill the new metaclass with properties, functions, etc.
     */
    template <typename T>
    static ClassBuilder<T> declare(const std::string& name);

public:

    /**
     * \brief Return the name of the metaclass
     *
     * \return String containing the name of the metaclass
     */
    CAMP_API const std::string& name() const;

    /**
     * \brief Return the total number of base metaclasses of this metaclass
     *
     * \return Number of base metaclasses
     */
    CAMP_API std::size_t baseCount() const;

    /**
     * \brief Return a base metaclass from its index
     *
     * \param index Index of the base to get
     *
     * \return Reference to the index-th base metaclass of this metaclass
     *
     * \throw camp::InvalidIndex index is out of range
     */
    CAMP_API const Class& base(std::size_t index) const;

    /**
     * \brief Return the total number of functions of this metaclass
     *
     * \return Number of functions
     */
    CAMP_API std::size_t functionCount() const;

    /**
     * \brief Check if this metaclass contains the given function
     *
     * \param name Name of the function to check
     *
     * \return True if the function is in the metaclass, false otherwise
     */
    CAMP_API bool hasFunction(const std::string& name) const;

    /**
     * \brief Get a function from its index in this metaclass
     *
     * \param index Index of the function to get
     *
     * \return Reference to the function
     *
     * \throw camp::InvalidIndex index is out of range
     */
    CAMP_API const Function& function(std::size_t index) const;

    /**
     * \brief Get a function from its name
     *
     * \param name Name of the function to get (case sensitive)
     *
     * \return Reference to the function
     *
     * \throw camp::InvalidFunction name doesn't exist in the metaclass
     */
    CAMP_API const Function& function(const std::string& name) const;

    /**
     * \brief Return the total number of properties of this metaclass
     *
     * \return Number of properties
     */
    CAMP_API std::size_t propertyCount() const;

    /**
     * \brief Check if this metaclass contains the given property
     *
     * \param name Name of the property to check
     *
     * \return True if the property is in the metaclass, false otherwise
     */
    CAMP_API bool hasProperty(const std::string& name) const;

    /**
     * \brief Get a property from its index in this metaclass
     *
     * \param index Index of the property to get
     *
     * \return Reference to the property
     *
     * \throw camp::InvalidIndex index is out of range
     */
    CAMP_API const Property& property(std::size_t index) const;

    /**
     * \brief Get a property from its name
     *
     * \param name Name of the property to get (case sensitive)
     *
     * \return Reference to the property
     *
     * \throw camp::InvalidProperty name doesn't exist in the metaclass
     */
    CAMP_API const Property& property(const std::string& name) const;

    /**
     * \brief Construct a new instance of the C++ class bound to the metaclass
     *
     * The template parameter T is the target C++ type. It can be the class bound
     * to the metaclass, one of its base classes, or void. The returned instance
     * must be destroyed with the Class::destroy function.
     *
     * \param args Arguments to pass to the constructor (empty by default)
     *
     * \return Pointer to the new instance, or 0 if it failed
     */
    template <typename T>
    T* construct(const Args& args = Args::empty) const;

    /**
     * \brief Destroy an instance of the C++ class bound to the metaclass
     *
     * This function must be called to destroy every instance created with
     * Class::construct. The object to destroy must be properly typed
     * (i.e. not be void*) so that its destructor gets called
     *
     * \param object Pointer to the object to destroyed
     *
     * \return Pointer to the new instance, or 0 if it failed
     */
    template <typename T>
    void destroy(const T* object) const;

    /**
     * \brief Start visitation of a class
     *
     * \param visitor Visitor to use for visitation
     */
    CAMP_API void visit(ClassVisitor& visitor) const;

    /**
     * \brief Operator == to check equality between two metaclasses
     *
     * Two metaclasses are equal if their name is the same.
     *
     * \param other Metaclass to compare with this
     *
     * \return True if both metaclasses are the same, false otherwise
     */
    CAMP_API bool operator==(const Class& other) const;

    /**
     * \brief Operator != to check inequality between two metaclasses
     *
     * \param other Metaclass to compare with this
     *
     * \return True if metaclasses are different, false if they are equal
     */
    CAMP_API bool operator!=(const Class& other) const;

private:

    template <typename T> friend class ClassBuilder;
    friend class detail::ClassManager;
    friend class UserObject;

    /**
     * \brief Construct the metaclass from its name
     *
     * \param name Name of the metaclass
     */
    CAMP_API Class(const std::string& name);

    /**
     * \brief Convert a pointer to an object to be compatible with a base metaclass
     *
     * \param pointer Pointer to convert
     * \param base Target base metaclass
     *
     * \return True if conversion was properly done, false on error
     */
    CAMP_API bool applyOffset(void*& pointer, const Class& base) const;

private:

    /**
     * \brief Structure holding informations about a base metaclass
     */
    struct BaseInfo
    {
        const Class* base;
        int offset;
    };

    typedef boost::shared_ptr<Property> PropertyPtr;
    typedef std::map<std::string, PropertyPtr> PropertyTable;

    typedef boost::shared_ptr<Function> FunctionPtr;
    typedef std::map<std::string, FunctionPtr> FunctionTable;

    typedef boost::shared_ptr<Constructor> ConstructorPtr;
    typedef std::vector<ConstructorPtr> ConstructorList;

    std::string m_name; ///< Name of the metaclass
    std::vector<BaseInfo> m_bases; ///< List of base metaclasses
    FunctionTable m_functions; ///< List of metafunctions
    PropertyTable m_properties; ///< List of metaproperties
    ConstructorList m_constructors; ///< List of metaconstructors
};

} // namespace camp

// Must be included here because of mutual dependance between Class and ClassBuilder
#include <camp/classbuilder.hpp>

#include <camp/class.inl>


#endif // CAMP_CLASS_HPP