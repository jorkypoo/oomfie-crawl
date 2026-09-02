#include "../inc/core.h"

/* mixer shit */

void init_app_mixers(Application* app) {
  // init mixer
  MIX_Mixer* dest1 = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
  if (!dest1) {
    SDL_Log("could not create a mixer from default audio device: %s", SDL_GetError());
    return;
  }
  
  app->mixer = dest1;

  // init tracks - one music & 2 misc
}

void free_app_mixers(Application* app) {
  MIX_DestroyMixer(app->mixer);
  MIX_DestroyTrack(app->music);
  MIX_DestroyTrack(app->misc1);
  MIX_DestroyTrack(app->misc2);
}
