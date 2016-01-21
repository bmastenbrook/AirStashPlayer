/*
 * This file is part of AirStashPlayer.
 * Copyright (c) 2016 Wearable Inc.
 *
 * AirStashPlayer is based on ijkPlayer:
 * Copyright (c) 2013-2014 Zhang Rui <bbcallen@gmail.com>
 *
 * Portions of ijkPlayer are based on kxmovie:
 * Copyright (c) 2012 Konstantin Boukreev. All rights reserved.
 *
 * AirStashPlayer is free software: you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU Lesser General
 * Public License as published by the Free Software Foundation.
 *
 * AirStashPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with AirStashPlayer.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

package com.airstash.player.player;

import com.airstash.player.player.misc.IMediaDataSource;

@SuppressWarnings("WeakerAccess")
public abstract class AbstractMediaPlayer implements IMediaPlayer {
    private OnPreparedListener mOnPreparedListener;
    private OnCompletionListener mOnCompletionListener;
    private OnBufferingUpdateListener mOnBufferingUpdateListener;
    private OnSeekCompleteListener mOnSeekCompleteListener;
    private OnVideoSizeChangedListener mOnVideoSizeChangedListener;
    private OnErrorListener mOnErrorListener;
    private OnInfoListener mOnInfoListener;

    public final void setOnPreparedListener(OnPreparedListener listener) {
        mOnPreparedListener = listener;
    }

    public final void setOnCompletionListener(OnCompletionListener listener) {
        mOnCompletionListener = listener;
    }

    public final void setOnBufferingUpdateListener(
            OnBufferingUpdateListener listener) {
        mOnBufferingUpdateListener = listener;
    }

    public final void setOnSeekCompleteListener(OnSeekCompleteListener listener) {
        mOnSeekCompleteListener = listener;
    }

    public final void setOnVideoSizeChangedListener(
            OnVideoSizeChangedListener listener) {
        mOnVideoSizeChangedListener = listener;
    }

    public final void setOnErrorListener(OnErrorListener listener) {
        mOnErrorListener = listener;
    }

    public final void setOnInfoListener(OnInfoListener listener) {
        mOnInfoListener = listener;
    }

    public void resetListeners() {
        mOnPreparedListener = null;
        mOnBufferingUpdateListener = null;
        mOnCompletionListener = null;
        mOnSeekCompleteListener = null;
        mOnVideoSizeChangedListener = null;
        mOnErrorListener = null;
        mOnInfoListener = null;
    }

    protected final void notifyOnPrepared() {
        if (mOnPreparedListener != null)
            mOnPreparedListener.onPrepared(this);
    }

    protected final void notifyOnCompletion() {
        if (mOnCompletionListener != null)
            mOnCompletionListener.onCompletion(this);
    }

    protected final void notifyOnBufferingUpdate(int percent) {
        if (mOnBufferingUpdateListener != null)
            mOnBufferingUpdateListener.onBufferingUpdate(this, percent);
    }

    protected final void notifyOnSeekComplete() {
        if (mOnSeekCompleteListener != null)
            mOnSeekCompleteListener.onSeekComplete(this);
    }

    protected final void notifyOnVideoSizeChanged(int width, int height,
                                                  int sarNum, int sarDen) {
        if (mOnVideoSizeChangedListener != null)
            mOnVideoSizeChangedListener.onVideoSizeChanged(this, width, height,
                    sarNum, sarDen);
    }

    protected final boolean notifyOnError(int what, int extra) {
        return mOnErrorListener != null && mOnErrorListener.onError(this, what, extra);
    }

    protected final boolean notifyOnInfo(int what, int extra) {
        return mOnInfoListener != null && mOnInfoListener.onInfo(this, what, extra);
    }

    public void setDataSource(IMediaDataSource mediaDataSource) {
        throw new UnsupportedOperationException();
    }
}
