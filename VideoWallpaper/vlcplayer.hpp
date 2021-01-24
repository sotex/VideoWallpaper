#ifndef VLCPALYER_HPP
#define VLCPALYER_HPP

#include "wallpaperwindow.hpp"

#include <string>
#include <map>

struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_media_t;
struct libvlc_event_t;

class VlcPlayer
{
public:
    static std::string version();
    static std::string complier_version();

public:
	VlcPlayer();
	~VlcPlayer();
	// 初始化播放器
	bool init();
	// 开始播放
    bool startPlay(std::string mediaPath,WINDOWID hwnd);
	// 停止播放
	void stopPlay();
	// 静音
	void setMute(bool mute = true);
	// 设置音量
	void setVolume(int volume);
	// 设置播放速率
	bool setPlayRate(float rate);
	// 设置播放位置(进度,0.0-1.0)
	bool setPlayPos(float pos);
	// 暂停
	void pausePlay();
	// 继续播放
	void continuePlay();
	// 是否正在播放
	bool isPlaying();

	/// ********************************************************************************
	/// <summary>
	/// 获取媒体总时长(单位:毫秒)
	/// </summary>
	/// <returns>媒体总时长毫秒数</returns>
	/// <created>ymwh@foxmail.com,2018/3/13</created>
	/// ********************************************************************************
	int64_t getMediaTimeLength();
	/// ********************************************************************************
	/// <summary>
	/// 获取媒体当前播放到的位置(已经播放毫秒数)
	/// </summary>
	/// <returns>当前播放毫秒数</returns>
	/// <created>ymwh@foxmail.com,2018/3/13</created>
	/// ********************************************************************************
	int64_t getMediaPlayTime();
	/// ********************************************************************************
	/// <summary>
	/// 获取媒体当前播放到的位置
	/// </summary>
	/// <returns>已经播放部分占全部比例，0.0-1.0之间</returns>
	/// <created>ymwh@foxmail.com,2018/3/13</created>
	/// ********************************************************************************
	float getMediaPlayPosition();
	/// ********************************************************************************
	/// <summary>
	/// 设置媒体播放的位置(跳转到指定比例位置继续播放)
	/// </summary>
	/// <param name="pos">指定跳转到的位置,0.0-1.0之间</param>
	/// <created>ymwh@foxmail.com,2018/3/13</created>
	/// ********************************************************************************
	void setMediaPlayPosition(float pos);

	std::map<int,std::string> getMediaAudioTracks();

	/// ********************************************************************************
	/// <summary>
	/// 获取播放媒体的当前音频轨道号.
	/// </summary>
	/// <returns>音频轨道数</returns>
	/// <created>ymwh@foxmail.com,2018/7/31</created>
	/// ********************************************************************************
	int getMediaCurrentAudioTrack();
	/// ********************************************************************************
	/// <summary>
	/// 设置播放媒体的音频轨道.
	/// </summary>
	/// <param name="track">轨道ID</param>
	/// <created>ymwh@foxmail.com,2018/7/31</created>
	/// ********************************************************************************
	void setMediaAudioTrack(int track);
	// SetPlayerPosChangedCallback()

	const std::string getLastError()
	{
		return m_error_msg;
	}
private:
	friend void handleEvents(const libvlc_event_t *event, void *userData);
private:
	struct libvlc_instance_t*       m_vlcInst;  // vlc实例
	struct libvlc_media_player_t*   m_vlcMplay; // vlc媒体播放器对象
	struct libvlc_media_t*          m_vlcMedia; // 媒体对象
	bool	m_bPlaying;		// 是否正在播放
	int     m_iVolume;		// 音量设置
	std::string m_error_msg;
};

#endif //!VLCPALYER_HPP
