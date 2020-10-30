#ifndef BASICRESOURCES_H
#define BASICRESOURCES_H

#include <map>
namespace Course {

/**
 * @brief BasicResource is an enumeration for different basic resource-types
 */
enum BasicResource {
    NONE = 0,
    MONEY = 1,
    FOOD = 2,
    WOOD = 3,
    STONE = 4,
    ORE = 5,
};

/**
 * @brief ResourceMap is an alias for std::map<BasicResource, int>
 */
using ResourceMap = std::map<BasicResource, int>;
/**
 * @brief ResourceMapDouble is an alias for std::map<Basicresource, double>
 */
using ResourceMapDouble = std::map<BasicResource, double>;


/**
 * @brief Creates a new ResourceMap that contains summed values of two
 * ResourceMaps
 * @param left first ResourceMap
 * @param right second ResourceMap
 * @return ResourceMap that has summed values from the two ResourceMaps
 */
ResourceMap mergeResourceMaps(const ResourceMap& left,
                              const ResourceMap& right);

/**
 * @brief Creates a new ResourceMap that has multiplied resmap values with
 * multmap values for the same key.
 * @param resmap ResourceMap which values are being multiplied.
 * @param multmap ResourceMapDouble that has multiplier values.
 * @return ResourceMap that has values of resmap multiplied by values of multmap
 */
ResourceMap multiplyResourceMap(const ResourceMap& resmap,
                                const ResourceMapDouble& multmap);

/**
 * @brief Creates a new ResourceMapDouble that has summed values of two
 * ResourceMapDoubles
 * @param left first ResourceMapDouble
 * @param right second ResourceMapDouble
 * @return ResourceMapDouble that has summed values from two ResourceMapDoubles
 */
ResourceMapDouble mergeResourceMapDoubles(const ResourceMapDouble& left,
                                          const ResourceMapDouble& right);

/**
 * @brief Creates a new ResourceMapDouble that has multiplied values of two
 * ResourceMapDoubles
 * @param left first ResourceMapDouble
 * @param right second ResourceMapDouble
 * @return ResourceMapDouble that has multiplied values from two
 * ResourceMapDoubles
 */
ResourceMapDouble multiplyResourceMapDoubles(const ResourceMapDouble& left,
                                             const ResourceMapDouble& right);

}

#endif // BASICRESOURCES_H
