/*
 * File: ScoreCard.cs
 * Brief: Handles changing scorecard information
 * Author: Peter Gortemaker
 */

using UnityEngine;
using TMPro;

public class ScoreCard : MonoBehaviour
{
    public GameManager gameManager;

    public TMP_Text totalText;
    public TMP_Text totalTextScore;
    public TMP_Text breakdownFirstFive;
    public TMP_Text breakdownLastFour;
    private int totalPar = 0;

    // Called by GameManager
    // Fills out the total Par (I made it this way in case I changed the par)
    // Fills out your score in comparison per hole.
    public void FillScoreCard()
    {
        totalPar = 0;
        for (int i = 0; i < gameManager.par.Length; i++)
        {
            totalPar += gameManager.par[i];
        }

        int index = gameManager.currentHole;
        string breakdown5 = "";
        string breakdown4 = "";
        for (int i = 0; i < index; i++)
        {
            if(i > GameManager.totalHoles / 2)
                breakdown4 += $"Hole {i + 1} - Par {gameManager.par[i]}: {gameManager.strokesPerHole[i]} stroke(s)\n";
            else
                breakdown5 += $"Hole {i + 1} - Par {gameManager.par[i]}: {gameManager.strokesPerHole[i]} stroke(s)\n";
        }

        totalText.text = $"Par({totalPar}) / Total Strokes({gameManager.GetTotalStrokes()}) = ";
        totalTextScore.text = $"Score: {gameManager.GetTotalStrokes() - totalPar}";
        breakdownFirstFive.text = breakdown5;
        breakdownLastFour.text = breakdown4;
    }
}