/*
 * File: MenuManager.cs
 * Brief: Main menu - difficulty slider saved/loaded with PlayerPrefs
 * Author: Peter Gortemaker
 */

using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using TMPro;

public class MenuManager : MonoBehaviour
{
    [SerializeField] private Slider diffSlider;
    [SerializeField] private TMP_Text diffText;

    private string[] diffNames =
    {
        "Normal",
        "Extra Bouncy",
        "Windy",
        "Both"
    };

    void Start()
    {
        Application.targetFrameRate = 60; 

        // Restore slider to last saved position
        diffSlider.value = PlayerPrefs.GetFloat("Difficulty", 0f);
        UpdateText();
    }

    public void OnSliderChanged()
    {
        UpdateText();
    }

    // Tells you what the difficulty change does
    void UpdateText()
    {
        int index = Mathf.RoundToInt(diffSlider.value);
        diffText.text = diffNames[index];
    }

    // To Play button
    public void StartGame()
    {
        PlayerPrefs.SetInt("Difficulty", Mathf.RoundToInt(diffSlider.value));
        SceneManager.LoadScene("SampleScene");
    }

    // To Quit button
    public void QuitGame()
    {
        Application.Quit();
    }
}