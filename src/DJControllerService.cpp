#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here
    std::string title = track.get_title();

    if (cache.contains(title)){
        cache.get(title); // MRU is updated in the function get() in "LRUCache"
        return 1;
    }

    PointerWrapper<AudioTrack> cloned_track = track.clone();
    if(cloned_track.get() == nullptr){
        std::cerr << "[ERROR] clone() returned nullptr\n";
        return 0;
    }

    cloned_track->load(); //the function exist at "LRUCache"
    cloned_track->analyze_beatgrid();  //the function exist at "LRUCache"

    bool is_evicted = cache.put(std::move(cloned_track));

    if(is_evicted){
        return -1;
    }
    return 0; // Placeholder
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
    AudioTrack* track_ptr = cache.get(track_title);

    if(track_ptr == nullptr){
        std::cerr << "[ERROR] Track: \"" << track_title << "\" not found in cache\n";
    }
    return track_ptr; // Placeholder
}
