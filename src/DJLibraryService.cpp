#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include "Playlist.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    for(size_t i = 0; i < library_tracks.size(); i++){
        AudioTrack* created_track = nullptr;
        if(library_tracks[i].type == "MP3"){
            created_track = new MP3Track(library_tracks[i].title, library_tracks[i].artists, library_tracks[i].duration_seconds, library_tracks[i].bpm, library_tracks[i].extra_param1, library_tracks[i].extra_param2);
        }

        else if(library_tracks[i].type == "WAV"){
            created_track = new WAVTrack(library_tracks[i].title, library_tracks[i].artists, library_tracks[i].duration_seconds, library_tracks[i].bpm, library_tracks[i].extra_param1, library_tracks[i].extra_param2);

        }

        if(created_track != nullptr){
            library.push_back(created_track);
        }
        else {
            std::cout << "[WARNING] Unsupported track format: " << library_tracks[i].type << "\n";
        }
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded\n";

}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    AudioTrack* track_found = playlist.find_track(track_title);
    if(track_found != nullptr){
        return track_found;
    }
    return nullptr; // Placeholder
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name << "\n";

    playlist = Playlist(playlist_name);
    int added_size = 0;

    for(size_t i = 0; i < track_indices.size(); i++){
        int idx = track_indices[i];
        if(idx <= 0 || static_cast<size_t>(idx) > library.size()){
            std::cout << "[WARNING] Invalid track index: " << idx << "\n";
            continue;
        }

        AudioTrack* library_track = library[idx - 1];
        PointerWrapper<AudioTrack> cloned_track = library_track->clone();
        if (cloned_track.get() == nullptr) {
            std::cout << "[ERROR] Clone failed for '" << library_track->get_title() << "', skipping\n";
            continue;
        }

        cloned_track->load();
        cloned_track->analyze_beatgrid();

        AudioTrack* raw_clone_track = cloned_track.release(); // unwraping cloned_track from pointerWrraper to AudioTrack using release
        playlist.add_track(raw_clone_track);

        added_size++;
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << added_size << " tracks)\n";
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles;

    PlaylistNode* curr = playlist.getHead(); 
    while (curr != nullptr) {
        if (curr->track) {
            titles.push_back(curr->track->get_title());
        }
        curr = curr->next;
    }

    return titles;
}