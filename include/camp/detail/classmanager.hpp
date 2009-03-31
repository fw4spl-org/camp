// <insert copyright>

#ifndef CAMP_DETAIL_CLASSMANAGER_HPP
#define CAMP_DETAIL_CLASSMANAGER_HPP


#include <camp/config.hpp>
#include <camp/detail/observernotifier.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <vector>


namespace camp
{
class Class;

namespace detail
{
/**
 * \brief Manages creation, storage, retrieval and destruction of metaclasses
 *
 * camp::ClassManager is the place where all metaclasses are stored and accessed.
 * It consists of a singleton which is created on first use and destroyed at global exit.
 *
 * \sa Class
 */
class ClassManager : public ObserverNotifier, boost::noncopyable
{
public:

    /**
     * \brief Get the unique instance of the class
     *
     * \return Reference to the unique instance of ClassManager
     */
    CAMP_API static ClassManager& instance();

    /**
     * \brief Create and register a new metaclass
     *
     * This is the entry point for every metaclass creation. This
     * function also notifies registered observers after successful creations.
     *
     * \param name Name of the metaclass to create (must be unique)
     * \param id Identifier of the C++ class bound to the metaclass
     *
     * \return Reference to the new metaclass
     */
    CAMP_API Class& registerNew(const std::string& name, const std::string& id);

    /**
     * \brief Get a metaclass from its name
     *
     * \param name Name of the metaclass to retrieve
     *
     * \return Reference to the requested metaclass
     *
     * \throw InvalidClass name is not the name of an existing metaclass
     */
    CAMP_API const Class& getByName(const std::string& name) const;

    /**
     * \brief Get the number of metaclasses bound to a C++ type
     *
     * \param id Identifier of the C++ type
     *
     * \return Number of metaclasses bound to the given type
     */
    CAMP_API std::size_t count(const std::string& id) const;

    /**
     * \brief Get a metaclass from a C++ type
     *
     * If multiple metaclasses are bound to the given type, the index parameter
     * is used to choose which one to return. Otherwise this parameter is ignored.
     *
     * \param id Identifier of the C++ type
     * \param index Index of the metaclass to get, within the metaclasses bound to the type
     *
     * \return Reference to the requested metaclass
     *
     * \throw InvalidClass id is not the name of an existing metaclass
     * \throw InvalidIndex index is out of range
     */
    CAMP_API const Class& getById(const std::string& id, std::size_t index) const;

    /**
     * \brief Get the total number of metaclasses
     *
     * \return Number of metaclasses that have been registered
     */
    CAMP_API std::size_t count() const;

    /**
     * \brief Get a metaclass from its global index
     *
     * This function, together with ClassManager::count, provides a way to iterate through
     * all the metaclasses that have been declared.
     *
     * \param index Global index of the metaclass to get
     *
     * \return Reference to the index-th metaclass
     *
     * \throw InvalidIndex index is out of range
     */
    CAMP_API const Class& getByIndex(std::size_t index) const;

    /**
     * \brief Check if a given type has a metaclass
     *
     * \param id Identifier of the C++ type
     *
     * \return True if the class exists, false otherwise
     */
    bool classExists(const std::string& id) const;

private:

    /**
     * \brief Default constructor
     */
    ClassManager();

    /**
     * \brief Destructor
     *
     * The destructor destroys all the registered metaclasses and notifies the observers.
     */
    ~ClassManager();

    typedef boost::shared_ptr<Class> ClassPtr;
    typedef std::map<std::string, ClassPtr> ClassByNameTable;
    typedef std::map<std::string, std::vector<ClassPtr> > ClassByIdTable;

    ClassByNameTable m_byName; ///< List of metaclasses sorted by name
    ClassByIdTable m_byId; ///< List of metaclasses sorted by class id
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_CLASSMANAGER_HPP