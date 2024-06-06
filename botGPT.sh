#!/bin/bash

# insert your openai API key here
API_KEY="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"
PROMPT="$1"

RESPONSE=$(curl -s -o response.json -w "%{http_code}" -X POST https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $API_KEY" \
  -d '{
  "model": "gpt-3.5-turbo",
  "messages": [{"role": "user", "content": "'"$PROMPT"'"}],
  "max_tokens": 50
}')

HTTP_STATUS=$(tail -n1 <<< "$RESPONSE")

if [ "$HTTP_STATUS" -eq 200 ]; then
  RESPONSE_TEXT=$(awk -v RS='[{}]' -v FS='[:,]' '/"content"/ {for (i=1; i<=NF; i++) if ($i ~ /"content"/) {print $(i+1)}}' response.json | sed 's/^"//; s/"$//')

  echo "$RESPONSE_TEXT" > response.txt
else
  echo "Failed to get a response. HTTP status code: $HTTP_STATUS"
  cat response.json
fi

rm response.json