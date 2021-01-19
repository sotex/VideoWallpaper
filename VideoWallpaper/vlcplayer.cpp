#include "stdafx.h"
#include "VlcPlayer.h"
#include <vlc/vlc.h>

VlcPlayer::VlcPlayer()
{
	m_vlcInst = NULL;
	m_vlcMplay = NULL;
	m_vlcMedia = NULL;

	m_bPlaying = false;
	m_iVolume = 60;
}


VlcPlayer::~VlcPlayer()
{
	stopPlay();
}

bool VlcPlayer::init()
{
	// 1、加载VLC引擎
	m_vlcInst = libvlc_new(0, NULL);
	if (m_vlcInst == NULL) {
		const char* msg = libvlc_errmsg();
		m_error_msg = msg ? msg : "Unkonwn Error";
		return false;
	}
	return true;
}

bool VlcPlayer::startPlay(std::string mediaPath, HWND hwnd)
{
	// 先停止现有的播放
	stopPlay();
	if (m_vlcInst == NULL) { return false; }

	// 2、创建媒体对象
	m_vlcMedia = libvlc_media_new_path(m_vlcInst, mediaPath.c_str());
	if (m_vlcMedia == NULL) { 
		goto FAILED_RETURN;
	}
	libvlc_media_parse(m_vlcMedia); // 解析一下媒体文件
	// 3、创建播放器
	m_vlcMplay = libvlc_media_player_new_from_media(m_vlcMedia);
	if (m_vlcMplay == NULL) {
		goto FAILED_RETURN;
	}
	// 4、设置播放输出窗体
	libvlc_media_player_set_hwnd(m_vlcMplay, hwnd);
	// 5、播放媒体
	if (-1 == libvlc_media_player_play(m_vlcMplay)) {
		goto FAILED_RETURN;
	}

	// https://blog.csdn.net/xgbm_k/article/details/78522640
	// 循环播放

	// 创建事件管理器
	//libvlc_event_manager_t*	m_eventManager = libvlc_media_player_event_manager(m_vlcMplay);
	// 监听事件
	//libvlc_event_attach(m_eventManager, libvlc_MediaPlayerPositionChanged, handleEvents, this);
	//libvlc_event_attach(m_eventManager, libvlc_MediaPlayerTimeChanged, handleEvents, this);

	// 设置音量
	libvlc_audio_set_volume(m_vlcMplay, m_iVolume);
	m_bPlaying = true;
	return true;

FAILED_RETURN:
	const char* msg = libvlc_errmsg();
	m_error_msg = msg ? msg : "Unkonwn Error";
	return false;
}

void VlcPlayer::stopPlay()
{
	//if (!m_bPlaying || m_vlcInst == NULL) {
		m_bPlaying = false;
	//}

	if (m_vlcMplay != NULL) {
		// 停止播放
		libvlc_media_player_stop(m_vlcMplay);
		// 销毁播放器
		libvlc_media_player_release(m_vlcMplay);
		m_vlcMplay = NULL;
	}
	if (m_vlcMedia != NULL) {
		// 销毁媒体对象
		libvlc_media_release(m_vlcMedia);
		m_vlcMedia = NULL;
	}
	m_bPlaying = false;
}

void VlcPlayer::setMute(bool mute)
{
	// 状态变化
	int Volume = mute ? 0 : m_iVolume;
	// 当前若是没有播放，退出
	if (!m_bPlaying) { return; }
	// 设置静音
	libvlc_audio_set_volume(m_vlcMplay, Volume);
}

void VlcPlayer::setVolume(int volume)
{
	if (m_vlcMplay == NULL) { return; }
	// 设置音量
	volume = volume < 0 ? 0 : (volume > 100 ? 100 : volume);
	libvlc_audio_set_volume(m_vlcMplay, volume);
}

bool VlcPlayer::setPlayRate(float rate)
{
	if (!m_bPlaying || m_vlcInst == NULL ||
		m_vlcMplay == NULL || m_vlcMedia == NULL) {
		return false;
	}
	if (-1 == libvlc_media_player_set_rate(m_vlcMplay, rate)) {
		const char* msg = libvlc_errmsg();
		m_error_msg = msg ? msg : "Unkonwn Error";
		return false;
	}
	return true;
}

bool VlcPlayer::setPlayPos(float pos)
{
	if (!m_bPlaying || m_vlcInst == NULL ||
		m_vlcMplay == NULL || m_vlcMedia == NULL) {
		return false;
	}
	libvlc_media_player_set_position(m_vlcMplay, pos);
	return false;
}

void VlcPlayer::pausePlay()
{
	if (!m_bPlaying || m_vlcInst == NULL ||
		m_vlcMplay == NULL || m_vlcMedia == NULL) {
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
	if (!m_bPlaying || m_vlcInst == NULL ||
		m_vlcMplay == NULL || m_vlcMedia == NULL) {
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

int64_t VlcPlayer::getMediaTimeLength()
{
	if (!m_bPlaying || m_vlcInst == NULL ||
		m_vlcMplay == NULL || m_vlcMedia == NULL) {
		return -1;
	}
	return libvlc_media_player_get_length(m_vlcMplay);
}

int64_t VlcPlayer::getMediaPlayTime()
{
	if (!m_bPlaying || m_vlcInst == NULL ||
		m_vlcMplay == NULL || m_vlcMedia == NULL) {
		return -1;
	}
	return libvlc_media_player_get_time(m_vlcMplay);
}

float VlcPlayer::getMediaPlayPosition()
{
	if (!m_bPlaying || m_vlcInst == NULL ||
		m_vlcMplay == NULL || m_vlcMedia == NULL) {
		return -1.0f;
	}
	return libvlc_media_player_get_position(m_vlcMplay);
}

void VlcPlayer::setMediaPlayPosition(float pos)
{
	if (!m_bPlaying || m_vlcInst == NULL ||
		m_vlcMplay == NULL || m_vlcMedia == NULL) {
		return;
	}
	if (pos < 0.0f || pos > 1.0f) { return; }
	libvlc_media_player_set_position(m_vlcMplay, pos);
}

std::map<int, std::string> VlcPlayer::getMediaAudioTracks()
{
	if (m_vlcMplay == NULL) { return std::map<int, std::string>(); }
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
	// libvlc_media_track_t **tracks = NULL;
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
	if (m_vlcMplay == NULL) { return; }
	// 设置音量
	libvlc_audio_set_track(m_vlcMplay, track);
}

int VlcPlayer::getMediaCurrentAudioTrack()
{
	if (m_vlcMplay == NULL) { return 0; }
	// 设置音量
	return libvlc_audio_get_track(m_vlcMplay);
}

void handleEvents(const libvlc_event_t * event, void* userData)
{
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
