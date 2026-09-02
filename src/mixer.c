#include "../inc/core.h"

/* mixer shit */

int init_app_mixers(Application* app) {
  // init mixer
  MIX_Mixer* dest = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
  if (!dest) {
    SDL_Log("could not create a mixer from default audio device: %s", SDL_GetError());
    return 0;
  }
  
  app->mixer = dest;

  // init tracks - one music & 2 misc
  MIX_Track* misc1 = MIX_CreateTrack(app->mixer);
  if (!misc1) {
    SDL_Log("could not create sdl track: %s", SDL_GetError());
    return 0;
  }

  MIX_Track* misc2 = MIX_CreateTrack(app->mixer);
  if (!misc2) {
    SDL_Log("could not create sdl track: %s", SDL_GetError());
    return 0;
  }

  MIX_Track* music = MIX_CreateTrack(app->mixer);
  if (!music) {
    SDL_Log("could not create sdl track: %s", SDL_GetError());
    return 0;
  }
  
  app->music_track_pos = 0;
  app->music_track = music;
  app->misc1_track = misc1;
  app->misc2_track = misc2;
  
  return 1;
}

void free_app_mixers(Application* app) {
  MIX_DestroyTrack(app->music_track);
  MIX_DestroyTrack(app->misc1_track);
  MIX_DestroyTrack(app->misc2_track);
  MIX_DestroyMixer(app->mixer);
}

void clear_app_mixers(Application* app) {
  MIX_SetTrackAudio(app->music_track, NULL);
  MIX_SetTrackAudio(app->misc1_track, NULL);
  MIX_SetTrackAudio(app->misc2_track, NULL);
}

void play_temp_audio(Application* app, char* path) {
  MIX_Audio* dest = MIX_LoadAudio(app->mixer, path, 1);
  if (!dest) return;

  MIX_PlayAudio(app->mixer, dest);
  MIX_DestroyAudio(dest);
}

void play_main_music(Application* app, char* path) {
  MIX_Audio* a = MIX_LoadAudio(app->mixer, path, 0);
  if (!a) return;

  // set the tracks current position to 0 - starting new track
  app->music_track_pos = 0;

  // make the track loop
  SDL_PropertiesID options = SDL_CreateProperties();
  if (!options) return;
  SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);

  if (MIX_TrackPlaying(app->music_track))
    MIX_SetTrackAudio(app->music_track, NULL);

  MIX_SetTrackAudio(app->music_track, a);
  MIX_PlayTrack(app->music_track, options);
  MIX_DestroyAudio(a);
}

void pause_main_music(Application* app, Sint64 ms) {
  if (MIX_TrackPlaying(app->music_track)) {
    // save where track should start up from in resume_main_music
    Sint64 t = MIX_GetTrackPlaybackPosition(app->music_track);
    if (t != -1) {
      t += MIX_TrackMSToFrames(app->music_track, ms);
      app->music_track_pos = t;
    } else {
      app->music_track_pos = 0;
    }
    
    MIX_StopTrack(app->music_track, MIX_TrackMSToFrames(app->music_track, ms));
  }
}

void resume_main_music(Application* app, Sint64 ms) {
  SDL_PropertiesID options = 0;
  if (!MIX_TrackPlaying(app->music_track)) {
    // start track from where it previously left off (if track was paused, that is)
    options = SDL_CreateProperties();
    SDL_SetNumberProperty(options, MIX_PROP_PLAY_START_FRAME_NUMBER, app->music_track_pos);
    // set the fade in time
    SDL_SetNumberProperty(options, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, ms);
    MIX_PlayTrack(app->music_track, options);
  }
}
