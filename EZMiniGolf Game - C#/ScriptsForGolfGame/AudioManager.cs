/*
 * File: AudioManager.cs
 * Brief: Audio manager for music and SFX
 * Author: Peter Gortemaker
 */

using UnityEngine;

public class AudioManager : MonoBehaviour
{
    [Header("Audio Sources")]
    [SerializeField] private AudioSource musicSource;
    [SerializeField] private AudioSource sfxSource;

    [Header("Audio Clips")]
    [SerializeField] private AudioClip backgroundMusic;
    [SerializeField] private AudioClip ballHitSound;
    [SerializeField] private AudioClip inHole;

    void Start()
    {
        musicSource.clip = backgroundMusic;
        musicSource.loop = true;
        musicSource.Play();
    }

    public void PlayBallHitSound() => sfxSource.PlayOneShot(ballHitSound);
    public void PlayInHoleSound() => sfxSource.PlayOneShot(inHole);
}