#include "vlcplayer.hpp"
#include <vlc/vlc.h>


// libvlc 下载地址 http://download.videolan.org/pub/videolan/vlc/

std::string VlcPlayer::version()
{
    return libvlc_get_version();
}

std::string VlcPlayer::complier_version()
{
    return libvlc_get_compiler();
}

VlcPlayer::VlcPlayer()
{
    m_vlcInst = nullptr;
    m_vlcMplay = nullptr;
    m_vlcMedia = nullptr;

    m_bPlaying = false;
    m_iVolume = 60;
}


VlcPlayer::~VlcPlayer()
{
    stopPlay();
    if(m_vlcInst != nullptr) {
        libvlc_release(m_vlcInst);
    }
}

bool VlcPlayer::init()
{
    // 1、加载VLC引擎
    m_vlcInst = libvlc_new(0, nullptr);
    if (m_vlcInst == nullptr) {
        const char* msg = libvlc_errmsg();
        m_errorMsg = msg ? msg : "Unkonwn Error";
        return false;
    }
    return true;
}

bool VlcPlayer::startPlay(std::string mediaPath, WINDOWID wid)
{
    // 先停止现有的播放
    stopPlay();
    if (m_vlcInst == nullptr) { return false; }

    // 2、创建媒体对象
    // 路径必须是 UTF-8 编码的
    m_vlcMedia = libvlc_media_new_path(m_vlcInst, mediaPath.c_str());
    if (m_vlcMedia == nullptr) {
        goto FAILED_RETURN;
    }
    libvlc_media_parse(m_vlcMedia); // 解析一下媒体文件
    // libvlc_media_add_option(m_vlcMedia, "–vout=directx");
    // 3、创建播放器
    m_vlcMplay = libvlc_media_player_new_from_media(m_vlcMedia);
    if (m_vlcMplay == nullptr) {
        goto FAILED_RETURN;
    }
    // 4、设置播放输出窗体
    libvlc_media_player_set_hwnd(m_vlcMplay, wid);
    // 5、播放媒体
    if (-1 == libvlc_media_player_play(m_vlcMplay)) {
        goto FAILED_RETURN;
    }

    // https://blog.csdn.net/xgbm_k/article/details/78522640
    // 循环播放

    // 获取事件管理器
    // libvlc_event_manager_t*	eventManager = libvlc_media_player_event_manager(m_vlcMplay);
    // 监听事件
    // libvlc_event_attach(eventManager,
    //     libvlc_MediaPlayerPositionChanged,
    //     handleEvents,
    //     this);

    // libvlc_event_attach(eventManager, libvlc_MediaPlayerTimeChanged, handleEvents, this);

    // 设置音量
    // libvlc_audio_set_volume(m_vlcMplay, m_iVolume);
    setVolume(m_iVolume);
    m_bPlaying = true;
    return true;

FAILED_RETURN:
    const char* msg = libvlc_errmsg();
    m_errorMsg = msg ? msg : "Unkonwn Error";
    return false;
}

void VlcPlayer::stopPlay()
{
    //if (!m_bPlaying || m_vlcInst == nullptr) {
    m_bPlaying = false;
    //}

    if (m_vlcMplay != nullptr) {
        // 停止播放
        libvlc_media_player_stop(m_vlcMplay);
        // 销毁播放器
        libvlc_media_player_release(m_vlcMplay);
        m_vlcMplay = nullptr;
    }
    if (m_vlcMedia != nullptr) {
        // 销毁媒体对象
        libvlc_media_release(m_vlcMedia);
        m_vlcMedia = nullptr;
    }
    m_bPlaying = false;
}

void VlcPlayer::setMute(bool mute)
{
    // 当前若是没有播放，退出
    if (!m_bPlaying) { return; }

    // 如果已经是静音状态，再设置静音
    // 或，反过来
    if(m_iVolume < 0 == mute) {
        return;
    }
    // 设置为正常音量状态
    if(m_iVolume < 0) {
        setVolume(-m_iVolume);
        return;
    }
    m_iVolume = -m_iVolume;
    // 设置静音
    libvlc_audio_set_volume(m_vlcMplay, 0);
}

void VlcPlayer::setVolume(int volume)
{
    if (m_vlcMplay == nullptr) { return; }
    // 设置音量
    m_iVolume = volume < 0 ? 0 : (volume > 100 ? 100 : volume);
    libvlc_audio_set_volume(m_vlcMplay, m_iVolume);
}

bool VlcPlayer::setPlayRate(float rate)
{
    if (!m_bPlaying || m_vlcInst == nullptr ||
        m_vlcMplay == nullptr || m_vlcMedia == nullptr) {
        return false;
    }
    if (-1 == libvlc_media_player_set_rate(m_vlcMplay, rate)) {
        const char* msg = libvlc_errmsg();
        m_errorMsg = msg ? msg : "Unkonwn Error";
        return false;
    }
    return true;
}

bool VlcPlayer::setPlayPos(float pos)
{
    if (!m_bPlaying || m_vlcInst == nullptr ||
        m_vlcMplay == nullptr || m_vlcMedia == nullptr) {
        return false;
    }
    libvlc_media_player_set_position(m_vlcMplay, pos);
    return false;
}

void VlcPlayer::pausePlay()
{
    if (!m_bPlaying || m_vlcInst == nullptr ||
        m_vlcMplay == nullptr || m_vlcMedia == nullptr) {
        return;
    }

    // 判断是否可以暂停播放
    if (!libvlc_media_player_can_pause(m_vlcMplay)) {
        return;
    }
    // 暂停播放
    libvlc_media_player_pause(m_vlcMplay);
}

void VlcPlayer::continuePlay()
{
    if (!m_bPlaying || m_vlcInst == nullptr ||
        m_vlcMplay == nullptr || m_vlcMedia == nullptr) {
        return;
    }
    if (libvlc_media_player_is_playing(m_vlcMplay) == 0) {
        // 继续播放
        libvlc_media_player_play(m_vlcMplay);
    }
}

bool VlcPlayer::isPlaying()
{
    return m_bPlaying && libvlc_media_player_is_playing(m_vlcMplay);
    //return m_bPlaying;
}

int VlcPlayer::getVolume() const
{
    return m_iVolume > 0? m_iVolume:0;
}

int64_t VlcPlayer::getMediaTimeLength()
{
    if (!m_bPlaying || m_vlcInst == nullptr ||
        m_vlcMplay == nullptr || m_vlcMedia == nullptr) {
        return -1;
    }
    return libvlc_media_player_get_length(m_vlcMplay);
}

int64_t VlcPlayer::getMediaPlayTime()
{
    if (!m_bPlaying || m_vlcInst == nullptr ||
        m_vlcMplay == nullptr || m_vlcMedia == nullptr) {
        return -1;
    }
    return libvlc_media_player_get_time(m_vlcMplay);
}

float VlcPlayer::getMediaPlayPosition()
{
    if (!m_bPlaying || m_vlcInst == nullptr ||
        m_vlcMplay == nullptr || m_vlcMedia == nullptr) {
        return -1.0f;
    }
    return libvlc_media_player_get_position(m_vlcMplay);
}

void VlcPlayer::setMediaPlayPosition(float pos)
{
    if (!m_bPlaying || m_vlcInst == nullptr ||
        m_vlcMplay == nullptr || m_vlcMedia == nullptr) {
        return;
    }
    if (pos < 0.0f || pos > 1.0f) { return; }
    libvlc_media_player_set_position(m_vlcMplay, pos);
}

std::map<int, std::string> VlcPlayer::getMediaAudioTracks()
{
    if (m_vlcMplay == nullptr) { return std::map<int, std::string>(); }
    /*libvlc_media_parse(m_vlcMedia);*/

    std::map<int, std::string> audiotrtacks;
    int count = libvlc_audio_get_track_count(m_vlcMplay);
    libvlc_track_description_t* tracks = libvlc_audio_get_track_description(m_vlcMplay);
    libvlc_track_description_t* pt = tracks;
    while (pt) {
        std::string trackname(pt->psz_name);
        //if (!trackname.empty() && trackname != "Disable") {
        audiotrtacks[pt->i_id] = trackname;
        //}
        pt = pt->p_next;
    }
    libvlc_track_description_list_release(tracks);
    // https://blog.csdn.net/liang19890820/article/details/80503302
    // 用于存储媒体轨迹的信息
    // libvlc_media_track_t **tracks = nullptr;
    // unsigned tracksCount = 0;
    // std::vector<int> audiochannels;
    // // 获取轨道信息
    // tracksCount = libvlc_media_tracks_get(m_vlcMedia, &tracks);
    // for (unsigned i = 0; i < tracksCount; ++i) {
    // 	libvlc_media_track_t *track = tracks[i];
    // 	// 音频轨道
    // 	if (track->i_type == libvlc_track_audio) {
    // 		libvlc_audio_track_t *audioTrack = track->audio;
    // 		audiochannels.push_back(audioTrack->i_channels);
    // 	}
    // }
    // libvlc_media_tracks_release(tracks, tracksCount);
    return audiotrtacks;
}

void VlcPlayer::setMediaAudioTrack(int track)
{
    if (m_vlcMplay == nullptr) { return; }
    // 设置音量
    libvlc_audio_set_track(m_vlcMplay, track);
}

int VlcPlayer::getMediaCurrentAudioTrack()
{
    if (m_vlcMplay == nullptr) { return 0; }
    // 设置音量
    return libvlc_audio_get_track(m_vlcMplay);
}


void handleEvents(const libvlc_event_t * event, void* userData)
{
    VlcPlayer* v = reinterpret_cast<VlcPlayer*>(userData);
    switch (event->type) {
        // media player 位置改变
    case libvlc_MediaPlayerPositionChanged: {
        break;
    }
    // media player 时间改变
    case libvlc_MediaPlayerTimeChanged: {
        break;
    }
    case libvlc_MediaPlayerStopped:
        break;
    default:
        break;
    }
}
