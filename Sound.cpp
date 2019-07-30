#include "Sound.h"
#include <string>
#include <iostream>
using namespace std;

Sound::Sound()
{
    m_DirectSound = 0;
    m_primaryBuffer = 0;
    m_bgmBuffers = 0;
	current_bgm = -1;
	num_bgm = 0;

    m_seBuffers = 0;
	num_se = 0;
	currentLaser = -1;
	currentHit = -1;
}


Sound::Sound(const Sound& other)
{
}


Sound::~Sound()
{
}


bool Sound::Initialize(HWND hwnd)
{
    bool result;

    // Initialize direct sound and the primary sound buffer.
    result = InitializeDirectSound(hwnd);
    if (!result)
    {
        return false;
    }
	
	num_bgm = 3;
    m_bgmBuffers = new IDirectSoundBuffer8*[num_bgm];
	LoadWaveFile("Sounds/Intro.wav", &m_bgmBuffers[0]);
	LoadWaveFile("Sounds/Corneria.wav", &m_bgmBuffers[1]);
	LoadWaveFile("Sounds/Credits.wav", &m_bgmBuffers[2]);

	for (int i = 0; i < num_bgm; i++)
		m_bgmBuffers[i]->SetVolume(-1200);


	num_se = 19;
	m_seBuffers = new IDirectSoundBuffer8*[num_se];
	LoadWaveFile("Sounds/LaserShot.wav", &m_seBuffers[0]);
	LoadWaveFile("Sounds/LaserShot.wav", &m_seBuffers[1]);
	LoadWaveFile("Sounds/LaserShot.wav", &m_seBuffers[2]);
	LoadWaveFile("Sounds/LaserShot.wav", &m_seBuffers[3]);
	LoadWaveFile("Sounds/AsteroidHit.wav", &m_seBuffers[4]);
	LoadWaveFile("Sounds/AsteroidHit.wav", &m_seBuffers[5]);
	LoadWaveFile("Sounds/AsteroidHit.wav", &m_seBuffers[6]);
	LoadWaveFile("Sounds/AsteroidHit.wav", &m_seBuffers[7]);
	LoadWaveFile("Sounds/ShipHit.wav", &m_seBuffers[8]);
	LoadWaveFile("Sounds/BombExplode.wav", &m_seBuffers[9]);
	LoadWaveFile("Sounds/Zadek.wav", &m_seBuffers[10]);
	LoadWaveFile("Sounds/Emergency.wav", &m_seBuffers[11]);
	LoadWaveFile("Sounds/Incoming.wav", &m_seBuffers[12]);
	LoadWaveFile("Sounds/Goodluck.wav", &m_seBuffers[13]);
	LoadWaveFile("Sounds/Prepare.wav", &m_seBuffers[14]);
	LoadWaveFile("Sounds/Bomb.wav", &m_seBuffers[15]);
	LoadWaveFile("Sounds/YAY.wav", &m_seBuffers[16]);
	LoadWaveFile("Sounds/Aww.wav", &m_seBuffers[17]);
	LoadWaveFile("Sounds/EnemyShot.wav", &m_seBuffers[18]);

	for (int i = 0; i < num_se-1; i++)
		m_seBuffers[i]->SetVolume(-800);

    return true;
}


void Sound::Shutdown()
{
    // Release the background music buffer.
	for (int i = 0; i < num_bgm; i++)
		ShutdownWaveFile(&m_bgmBuffers[i]);
	if (m_bgmBuffers)
	{
		delete m_bgmBuffers;
		m_bgmBuffers = 0;
	}

    // Sound Effect buffers
	for (int i = 0; i < num_se; i++)
		ShutdownWaveFile(&m_seBuffers[i]);
	if (m_seBuffers)
	{
		delete m_seBuffers;
		m_seBuffers = 0;
	}

    // Shutdown the Direct Sound API.
    ShutdownDirectSound();

    return;
}


bool Sound::InitializeDirectSound(HWND hwnd)
{
    HRESULT result;
    DSBUFFERDESC bufferDesc;
    WAVEFORMATEX waveFormat;

    // Initialize the direct sound interface pointer for the default sound device.
    result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
    if (FAILED(result))
        return false;

    // Set the cooperative level to priority so the format of the primary sound buffer can be modified.
    result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
    if (FAILED(result))
        return false;

    // Setup the primary buffer description.
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
    bufferDesc.dwBufferBytes = 0;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = NULL;
    bufferDesc.guid3DAlgorithm = GUID_NULL;

    // Get control of the primary sound buffer on the default sound device.
    result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
    if (FAILED(result))
        return false;

    // Setup the format of the primary sound bufffer.
    // In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nChannels = 2;
    waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    // Set the primary buffer to be the wave format specified.
    result = m_primaryBuffer->SetFormat(&waveFormat);
    if (FAILED(result))
        return false;

    return true;
}


void Sound::ShutdownDirectSound()
{
    // Release the primary sound buffer pointer.
    if (m_primaryBuffer)
    {
        m_primaryBuffer->Release();
        m_primaryBuffer = 0;
    }

    // Release the direct sound interface pointer.
    if (m_DirectSound)
    {
//		m_DirectSound->Release();
        m_DirectSound = 0;
    }

    return;
}


bool Sound::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
    int error;
    FILE* filePtr;
    unsigned int count;
    WaveHeaderType waveFileHeader;
    WAVEFORMATEX waveFormat;
    DSBUFFERDESC bufferDesc;
    HRESULT result;
    IDirectSoundBuffer* tempBuffer;
    unsigned char* waveData;
    unsigned char* bufferPtr;
    unsigned long bufferSize;


    // Open the wave file in binary.
    error = fopen_s(&filePtr, filename, "rb");
    if (error != 0)
        return false;

    // Read in the wave file header.
    count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
    if (count != 1)
        return false;

    // Check that the chunk ID is the RIFF format.
    if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') || (waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
        return false;

    // Check that the file format is the WAVE format.
    if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') || (waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
        return false;

    // Check that the sub chunk ID is the fmt format.
    if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') || (waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
        return false;

    // Check that the audio format is WAVE_FORMAT_PCM.
    if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
        return false;

    // Check that the wave file was recorded in stereo format.
    if (waveFileHeader.numChannels != 2)
        return false;

    // Check that the wave file was recorded at a sample rate of 44.1 KHz.
    if (waveFileHeader.sampleRate != 44100)
        return false;

    // Ensure that the wave file was recorded in 16 bit format.
    if (waveFileHeader.bitsPerSample != 16)
        return false;

    // Check for the data chunk header.
    if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') || (waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
        return false;

    // Set the wave format of secondary buffer that this wave file will be loaded onto.
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nChannels = 2;
    waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    // Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
    bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = &waveFormat;
    bufferDesc.guid3DAlgorithm = GUID_NULL;

    // Create a temporary sound buffer with the specific buffer settings.
    result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
    if (FAILED(result))
        return false;

    // Test the buffer format against the direct sound 8 interface and create the secondary buffer.
    result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
    if (FAILED(result))
        return false;

    // Release the temporary buffer.
    tempBuffer->Release();
    tempBuffer = 0;

    // Move to the beginning of the wave data which starts at the end of the data chunk header.
    fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

    // Create a temporary buffer to hold the wave file data.
    waveData = new unsigned char[waveFileHeader.dataSize];
    if (!waveData)
        return false;

    // Read in the wave file data into the newly created buffer.
    count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
    if (count != waveFileHeader.dataSize)
        return false;

    // Close the file once done reading.
    error = fclose(filePtr);
    if (error != 0)
        return false;

    // Lock the secondary buffer to write wave data into it.
    result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
    if (FAILED(result))
        return false;

    // Copy the wave data into the buffer.
    memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

    // Unlock the secondary buffer after the data has been written to it.
    result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
    if (FAILED(result))
        return false;

    // Release the wave data since it was copied into the secondary buffer.
    delete[] waveData;
    waveData = 0;

    return true;
}


void Sound::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
    // Release the secondary sound buffer.
    if (*secondaryBuffer)
    {
        (*secondaryBuffer)->Release();
        *secondaryBuffer = 0;
    }

    return;
}


bool Sound::PlayBGM(int num)
{
    HRESULT result;
	StopBGM();

    // Play the contents of the secondary sound buffer.
	current_bgm = num;
	if (num >= 0)
	{
		result = m_bgmBuffers[num]->Play(0, 0, DSBPLAY_LOOPING);
		if (FAILED(result))
			return false;
	}
    return true;
}

void Sound::StopBGM()
{
	if (current_bgm >= 0)
	{
		m_bgmBuffers[current_bgm]->Stop();
		m_bgmBuffers[current_bgm]->SetCurrentPosition(175000 * 0);
	}
}

// Function will take in an integer value and play the particular sound file assoicated with the number
void Sound::PlaySE(int choice)
{
	if (choice < num_se)
	{
		if (choice == 0)
			choice = (++currentLaser) % 4;
		if (choice == 4)
			choice = (++currentHit) % 4 + 4;
		m_seBuffers[choice]->Play(0, 0, 0);
	}
	return;
}