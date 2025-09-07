from dotenv import load_dotenv
from groq import Groq
import os

class NEXAS:

    model_options = [
        "llama-3.2-3b-preview"
    ]

    def __init__(self, model):
        load_dotenv()
        self.client = Groq(api_key=os.getenv("GROQ_API_KEY"))
        self.model = model
        self.prompt = """## Role:
        You are a helpful assistant.

        ## Objective:
        You will be given a user utterance as input. Using the utterance classify the intention of the action. Then service the action accordingly.

        ## Classes:
        ```json
        {
            "Class Name": "Query",
            "Class Content": Respond to the query,
        },
        {
            "Class Name": "Action",
            "Class Content": Respond with what actions you would need to perform if you had access to a robot to control
        }
        ```

        ## Instructions:
        1. Identify which of the classes the utterance belongs to
        2. Depending on the class answer the class content

        ## Output:
        ```json
        {
            "class": selected class,
            "content": content corresponding to class
        }
        ```
        """
        
    def query(self, query):
        completion = self.client.chat.completions.create(
            model=self.model,
            messages=[
                {
                    "role": "system",
                    "content": self.prompt
                },
                {
                    "role": "user",
                    "content": query
                }
            ],
            temperature=1,
            max_tokens=1024,
            top_p=1,
            stream=True,
            stop=None,
        )

        for chunk in completion:
            print(chunk.choices[0].delta.content or "", end="")

def main():
    assistant = NEXAS(model="llama-3.2-3b-preview")
    assistant.query("What is the weather today")

if __name__ == "__main__":
    main()

