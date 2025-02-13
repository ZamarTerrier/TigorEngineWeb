#include "e_audioObject.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "e_memory.h"
#include "e_camera.h"

#include "e_math.h"

#define DELAY_IN_SECONDS    0.4f
#define DECAY               0.25f   /* Volume falloff for each echo. */

void AudioObjectInit(AudioObject* ao)
{
    ao->object = AllocateMemory(1, sizeof(ma_engine));
    ao->g_delayNode = AllocateMemory(1, sizeof(ma_delay_node));
    ao->volume = 0.1f;
    ao->pan = 1.0f;
    ao->pitch = 1.0f;

    ma_engine_init(NULL, ao->object);

    ma_result result;
    ma_uint32 channels;
    ma_uint32 sampleRate;

    ma_delay_node_config delayNodeConfig;
    channels   = ma_engine_get_channels(ao->object);
    sampleRate = ma_engine_get_sample_rate(ao->object);

    delayNodeConfig = ma_delay_node_config_init(channels, sampleRate, (ma_uint32)(sampleRate * DELAY_IN_SECONDS), DECAY);

    result = ma_delay_node_init(ma_engine_get_node_graph(ao->object), &delayNodeConfig, NULL, ao->g_delayNode);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize delay node.");
        return;
    }

    /* Connect the output of the delay node to the input of the endpoint. */
    ma_node_attach_output_bus(ao->g_delayNode, 0, ma_engine_get_endpoint(ao->object), 0);
}

void AudioObjectLoadFile(AudioObject* ao, const char* path)
{
    ma_result result;

    ao->num_sounds ++;

    ao->sounds = realloc(ao->sounds, ao->num_sounds * sizeof(ma_sound*));
    ao->sounds[ao->num_sounds - 1] = AllocateMemory(1, sizeof(ma_sound));

    result = ma_sound_init_from_file(ao->object, path, 0, NULL, NULL, ao->sounds[ao->num_sounds - 1]);
}

void AudioObjectJustPlaySound(AudioObject* ao, const char* path)
{
    ma_engine_play_sound(ao->object, path, NULL);
}

void AudioObjectPlaySound(AudioObject* ao, uint32_t num)
{
    if(num < ao->num_sounds)
    {
        //ma_node_attach_output_bus(ao->sounds[num], 0, ao->g_delayNode, 0);
        ma_sound_set_volume(ao->sounds[num], ao->volume);
        ma_sound_set_pan(ao->sounds[num], ao->pan);
        ma_sound_set_pitch(ao->sounds[num], ao->pitch);
        ma_sound_start(ao->sounds[num]);
    }
}

void AudioObjectSetPan(AudioObject *ao, float pan)
{
    ao->pan = pan;

    for(int i=0;i < ao->num_sounds;i++)
    {
        ma_sound_set_pan(ao->sounds[i], ao->pan);
    }
}

void AudioObjectSetVolume(AudioObject *ao, float volume)
{
    ao->volume = volume;

    for(int i=0;i < ao->num_sounds;i++)
    {
        ma_sound_set_volume(ao->sounds[i], ao->volume);
    }
}

void AudioObjectSetPitch(AudioObject *ao, float pitch)
{
    ao->pitch = pitch;

    for(int i=0;i < ao->num_sounds;i++)
    {
        ma_sound_set_pitch(ao->sounds[i], ao->pitch);
    }
}

void AudioObjectSetSourcePosition3D(AudioObject *ao, vec3 position)
{
    vec3 side = v3_cross((vec3){ 0, 1, 0}, v3_norm(Camera3DGetRotation()));
    side = v3_norm(side);
    vec3 razn = v3_sub(position, Camera3DGetPosition());
    float x_temp = v3_dot(razn, side);
    float pan = clamp(x_temp / 10.0f, -1, 1);

    float vol = 1 / v3_length(razn);//(cos(EngineGetTime()) + 1.0f) / 2.f

    AudioObjectSetPan(ao, -pan);
    AudioObjectSetVolume(ao, vol);
}

void AudioObjectStopSound(AudioObject* ao, uint32_t num)
{
    if(num < ao->num_sounds)
        ma_sound_stop(ao->sounds[num]);
}

void AudioObjectDestroy(AudioObject* ao)
{
    for(int i=0; i < ao->num_sounds;i++)
    {
        ma_sound_stop(ao->sounds[i]);
        ma_sound_uninit(ao->sounds[i]);
        FreeMemory(ao->sounds[i]);
    }
    FreeMemory(ao->sounds);

    ma_engine_uninit(ao->object);
    FreeMemory(ao->object);
    FreeMemory(ao->g_delayNode);
}
