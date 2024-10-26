from groq import Groq
import os
from dotenv import load_dotenv

load_dotenv()
groq_api_key = os.getenv("GROQ_API_KEY")

client = Groq(api_key=groq_api_key)
completion = client.chat.completions.create(
    model="llama-3.2-3b-preview",
    messages=[
        {
            "role": "system",
            "content": "given an input. write a haiku"
        },
        {
            "role": "user",
            "content": "moon"
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
