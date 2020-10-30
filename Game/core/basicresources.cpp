#include "basicresources.h"

namespace Course {


ResourceMap mergeResourceMaps(const ResourceMap& left,
                              const ResourceMap& right)
{
    ResourceMap new_map = left;

    for( auto left_it = left.begin(); left_it != left.end(); ++left_it)
    {
        auto right_it = right.find(left_it->first);
        if(right_it != right.end())
        {
            new_map[left_it->first] = left_it->second + right_it->second;
        }
    }

    for( auto right_it = right.begin(); right_it != right.end(); ++right_it)
    {
        if(new_map.find(right_it->first) == new_map.end())
        {
            new_map[right_it->first] = right_it->second;
        }
    }

    return new_map;
}

ResourceMap multiplyResourceMap(const ResourceMap& resmap,
                                const ResourceMapDouble& multmap)
{
    ResourceMap new_map = resmap;

    for( auto left_it = new_map.begin(); left_it != new_map.end(); ++left_it)
    {
        auto right_it = multmap.find(left_it->first);
        if(right_it != multmap.end())
        {
            new_map[left_it->first] = left_it->second * right_it->second;
        }
        else
        {
            new_map[left_it->first] = 0;
        }
    }

    return new_map;
}

ResourceMapDouble mergeResourceMapDoubles(const ResourceMapDouble& left,
                                          const ResourceMapDouble& right)
{
    ResourceMapDouble new_map = left;

    for( auto left_it = left.begin(); left_it != left.end(); ++left_it)
    {
        auto right_it = right.find(left_it->first);
        if( right_it != right.end())
        {
            new_map[left_it->first] = left_it->second + right_it->second;
        }
    }

    for( auto right_it = right.begin(); right_it != right.end(); ++right_it)
    {
        if(new_map.find(right_it->first) == new_map.end())
        {
            new_map[right_it->first] = right_it->second;
        }
    }

    return new_map;
}

ResourceMapDouble multiplyResourceMapDoubles(const ResourceMapDouble& left,
                                             const ResourceMapDouble& right)
{
    ResourceMapDouble new_map = left;

    for( auto left_it = left.begin(); left_it != left.end(); ++left_it)
    {
        auto right_it = right.find(left_it->first);
        if( right_it != right.end())
        {
            new_map[left_it->first] = left_it->second * right_it->second;
        }
        else
        {
            new_map[left_it->first] = 0.00;
        }
    }

    return new_map;
}


}
