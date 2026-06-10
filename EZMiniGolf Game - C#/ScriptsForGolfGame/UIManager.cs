/*
 * File: UIManager.cs
 * Brief: Made a seperate UI Manager so I didn't clutter GameManager up as much
 *          with the functionality I needed
 * Author: Peter Gortemaker
 */

using UnityEngine;
using TMPro;
using UnityEngine.UI;

public class UIManager : MonoBehaviour
{
    public GameManager gameManager;
    public Camera mainCamera;

    [Header("Info")]
    public TMP_Text holeText;
    public TMP_Text strokeText;

    [Header("Power Bar")]
    public Slider powerSlider;
    public TMP_Text powerText;

    [Header("Wind")]
    public RectTransform windArrow;
    public TMP_Text windText;

    [Header("Controls")]
    public GameObject controls;


    public void UpdateUI(int hole, int strokes)
    {
        holeText.text = "Hole " + hole;
        strokeText.text = "Strokes: " + strokes;
    }

    public void UpdatePower(float power, bool ballMoving)
    {
        powerSlider.value = power;
        powerText.text = "Power: " + Mathf.RoundToInt(power * 100) + "%";

        controls.SetActive(!ballMoving);
    }

    public void UpdateWind(Vector3 windForce)
    {
        if (windText)
            windText.text = $"Wind: {(windForce.magnitude * 20):F1} km/h"; // The multiplication here is just to help visualize intensity

        // I wanted the windArrow to have weathervane/intuitive compassing
        if (windArrow)
        {
            float worldAngle = Quaternion.LookRotation(windForce).eulerAngles.y;
            float cameraAngle = mainCamera.transform.eulerAngles.y;
            float relAngle = worldAngle - cameraAngle;
            windArrow.localEulerAngles = new Vector3(0f, 0f, -relAngle + 90f); // +90f because the arrow png faces right.
        }
    }

    void Update()
    {
        UpdateWind(gameManager.windManager.GetWindForce());
    }
}