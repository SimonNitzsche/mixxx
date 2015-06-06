#ifndef SOUNDSOURCEPROXY_H
#define SOUNDSOURCEPROXY_H

#include "trackinfoobject.h"

#include "sources/soundsourceproviderregistry.h"

#include "util/sandbox.h"

// Creates sound sources for filenames or tracks
class SoundSourceProxy: public Mixxx::MetadataSource {
public:
    static void loadPlugins(); // not thread-safe

    static QStringList getSupportedFileExtensions() {
        return s_soundSourceProviders.getRegisteredFileExtensions();
    }
    static QStringList getSupportedFileExtensionsByPlugins();
    static const QStringList& getSupportedFileNamePatterns() {
        return s_supportedFileNamePatterns;
    }
    static const QRegExp& getSupportedFileNamesRegex() {
        return s_supportedFileNamesRegex;
    }

    static bool isUrlSupported(const QUrl& url);
    static bool isFileSupported(const QFileInfo& fileInfo);
    static bool isFileNameSupported(const QString& fileName);
    static bool isFileExtensionSupported(const QString& fileExtension);

    explicit SoundSourceProxy(
            const QString& fileName,
            SecurityTokenPointer pSecurityToken = SecurityTokenPointer());
    explicit SoundSourceProxy(
            const TrackPointer& pTrack);

    QString getType() const {
        if (m_pSoundSource) {
            return m_pSoundSource->getType();
        } else {
            return QString();
        }
    }

    Result parseTrackMetadataAndCoverArt(
            Mixxx::TrackMetadata* pTrackMetadata,
            QImage* pCoverArt) const override {
        if (m_pSoundSource) {
            return m_pSoundSource->parseTrackMetadataAndCoverArt(
                    pTrackMetadata, pCoverArt);
        } else {
            return ERR;
        }
    }

    // Opening the audio data through the proxy will
    // update the some metadata of the track object.
    // Returns a null pointer on failure.
    Mixxx::AudioSourcePointer openAudioSource(const Mixxx::AudioSourceConfig& audioSrcCfg = Mixxx::AudioSourceConfig());

    void closeAudioSource();

private:
    static Mixxx::SoundSourceProviderRegistry s_soundSourceProviders;
    static QStringList s_supportedFileNamePatterns;
    static QRegExp s_supportedFileNamesRegex;

    const TrackPointer m_pTrack;
    const SecurityTokenPointer m_pSecurityToken;

    const QUrl m_url;

    static Mixxx::SoundSourceProviderRegistrationList findSoundSourceProviderRegistrations(const QUrl& url);

    const Mixxx::SoundSourceProviderRegistrationList m_soundSourceProviderRegistrations;
    int m_soundSourceProviderRegistrationIndex;

    Mixxx::SoundSourceProviderPointer getSoundSourceProvider() const;
    void nextSoundSourceProvider();

    void initSoundSource();

    Mixxx::SoundSourcePointer m_pSoundSource;

    // Just an alias that keeps track of opening and closing
    // the corresponding SoundSource.
    Mixxx::AudioSourcePointer m_pAudioSource;
};

#endif
