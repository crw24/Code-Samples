package hw1;
import java.io.*;
import java.util.Scanner;

public class hw1
{
	public static void hw1(String inFile, String outFile) throws IOException
	{
		boolean [] pangram = new boolean[26];

		File in = new File(inFile);
		File out = new File(outFile);
		FileWriter fileWriter = new FileWriter(out);
		
		Scanner fileScan = new Scanner(in);
		String currentLine;
		
		int charIndex;		
		while(fileScan.hasNextLine())
		{
			currentLine = fileScan.nextLine();
			for(int i = 97; i <= 122; i++)
			{
				charIndex = currentLine.indexOf(i);
				if(charIndex != -1)
				{
					pangram[i - 97] = true;
				}
				else
					pangram[i - 97] = false;
				
			}
			
			int allTrue = 0;
			for(int i = 0; i <= 25; i++)
			{
				if (pangram[i] == false)
				{
						fileWriter.write("false\n");
						i = 26;				}
				else if (pangram[i] == true)
					allTrue++;
			}
			if (allTrue == 26)
					fileWriter.write("true\n");
		}   
		fileScan.close();
		fileWriter.close();
	}
}
